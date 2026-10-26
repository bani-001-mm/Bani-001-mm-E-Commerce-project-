#include "http_server.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <algorithm>
#include <windows.h>

// -----------------------------------------------------------------------------
// HttpResponse Helpers
// -----------------------------------------------------------------------------
HttpResponse HttpResponse::json(int code, const std::string& jsonBody) {
    HttpResponse res;
    res.statusCode = code;
    res.statusText = (code == 200) ? "OK" : (code == 201 ? "Created" : "Error");
    res.headers["Content-Type"] = "application/json; charset=utf-8";
    res.headers["Access-Control-Allow-Origin"] = "*";
    res.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
    res.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization";
    res.body = jsonBody;
    return res;
}

HttpResponse HttpResponse::text(int code, const std::string& textBody) {
    HttpResponse res;
    res.statusCode = code;
    res.statusText = (code == 200) ? "OK" : "Error";
    res.headers["Content-Type"] = "text/plain; charset=utf-8";
    res.headers["Access-Control-Allow-Origin"] = "*";
    res.body = textBody;
    return res;
}

HttpResponse HttpResponse::file(int code, const std::string& mimeType, const std::string& data) {
    HttpResponse res;
    res.statusCode = code;
    res.statusText = "OK";
    res.headers["Content-Type"] = mimeType;
    res.headers["Access-Control-Allow-Origin"] = "*";
    res.body = data;
    return res;
}

HttpResponse HttpResponse::corsOptions() {
    HttpResponse res;
    res.statusCode = 204;
    res.statusText = "No Content";
    res.headers["Access-Control-Allow-Origin"] = "*";
    res.headers["Access-Control-Allow-Methods"] = "GET, POST, PUT, DELETE, OPTIONS";
    res.headers["Access-Control-Allow-Headers"] = "Content-Type, Authorization";
    res.headers["Access-Control-Max-Age"] = "86400";
    return res;
}

HttpResponse HttpResponse::notFound(const std::string& msg) {
    return HttpResponse::json(404, "{\"error\": \"" + JsonHelper::escape(msg) + "\"}");
}

HttpResponse HttpResponse::badRequest(const std::string& msg) {
    return HttpResponse::json(400, "{\"error\": \"" + JsonHelper::escape(msg) + "\"}");
}

// -----------------------------------------------------------------------------
// HttpServer Implementation
// -----------------------------------------------------------------------------
struct ClientContext {
    HttpServer* server;
    SOCKET clientSocket;
};

static DWORD WINAPI clientWorker(LPVOID lpParam) {
    ClientContext* ctx = reinterpret_cast<ClientContext*>(lpParam);
    if (ctx) {
        ctx->server->handleClient(ctx->clientSocket);
        delete ctx;
    }
    return 0;
}

HttpServer::HttpServer(int port, Database& db, const std::string& staticDir)
    : port_(port),
      db_(db),
      staticDir_(staticDir),
      running_(false),
      listenSocket_(INVALID_SOCKET) {
}

HttpServer::~HttpServer() {
    stop();
}

bool HttpServer::start() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "[HttpServer] WSAStartup failed: " << WSAGetLastError() << "\n";
        return false;
    }

    listenSocket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket_ == INVALID_SOCKET) {
        std::cerr << "[HttpServer] socket creation failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return false;
    }

    // Set SO_REUSEADDR
    char opt = 1;
    setsockopt(listenSocket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port_);

    if (bind(listenSocket_, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[HttpServer] Bind failed on port " << port_ << ": " << WSAGetLastError() << "\n";
        closesocket(listenSocket_);
        WSACleanup();
        return false;
    }

    if (listen(listenSocket_, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "[HttpServer] Listen failed: " << WSAGetLastError() << "\n";
        closesocket(listenSocket_);
        WSACleanup();
        return false;
    }

    running_ = true;
    std::cout << "========================================================\n"
              << " AuraStore C++ Backend Server Running!\n"
              << " Port: " << port_ << "\n"
              << " REST API Base: http://localhost:" << port_ << "/api\n"
              << " Web Frontend:  http://localhost:" << port_ << "/\n"
              << " Database Directory: data/\n"
              << "========================================================\n";

    while (running_) {
        sockaddr_in clientAddr;
        int clientLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket_, (sockaddr*)&clientAddr, &clientLen);

        if (clientSocket == INVALID_SOCKET) {
            if (running_) {
                std::cerr << "[HttpServer] Accept failed: " << WSAGetLastError() << "\n";
            }
            break;
        }

        // Spawn a thread for client connection via Windows native threading
        ClientContext* ctx = new ClientContext{ this, clientSocket };
        HANDLE hThread = CreateThread(NULL, 0, clientWorker, ctx, 0, NULL);
        if (hThread) {
            CloseHandle(hThread); // Detach handle, thread executes and cleans up context
        } else {
            handleClient(clientSocket);
            delete ctx;
        }
    }

    return true;
}

void HttpServer::stop() {
    if (running_) {
        running_ = false;
        if (listenSocket_ != INVALID_SOCKET) {
            closesocket(listenSocket_);
            listenSocket_ = INVALID_SOCKET;
        }
        WSACleanup();
        std::cout << "[HttpServer] Server stopped.\n";
    }
}

void HttpServer::handleClient(SOCKET clientSocket) {
    char buffer[4096];
    std::string rawRequest;
    size_t headerEnd = std::string::npos;
    size_t contentLength = 0;

    while (true) {
        int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) break;
        buffer[bytesRead] = '\0';
        rawRequest.append(buffer, bytesRead);

        if (headerEnd == std::string::npos) {
            headerEnd = rawRequest.find("\r\n\r\n");
            if (headerEnd != std::string::npos) {
                // Check for Content-Length header
                std::string lowerReq = rawRequest.substr(0, headerEnd);
                std::transform(lowerReq.begin(), lowerReq.end(), lowerReq.begin(), ::tolower);
                size_t clPos = lowerReq.find("content-length:");
                if (clPos != std::string::npos) {
                    size_t colon = rawRequest.find(':', clPos);
                    size_t lineEnd = rawRequest.find("\r\n", clPos);
                    if (colon != std::string::npos && lineEnd != std::string::npos) {
                        try {
                            contentLength = std::stoul(rawRequest.substr(colon + 1, lineEnd - colon - 1));
                        } catch (...) {}
                    }
                }
            }
        }

        if (headerEnd != std::string::npos) {
            size_t bodyBytesRead = rawRequest.length() - (headerEnd + 4);
            if (bodyBytesRead >= contentLength) {
                break;
            }
        }
    }

    if (!rawRequest.empty()) {
        HttpRequest req = parseRequest(rawRequest);
        HttpResponse res = routeRequest(req);
        std::string rawResponse = serializeResponse(res);
        send(clientSocket, rawResponse.c_str(), (int)rawResponse.length(), 0);
    }

    closesocket(clientSocket);
}

std::string HttpServer::urlDecode(const std::string& in) {
    std::string out;
    out.reserve(in.length());
    for (size_t i = 0; i < in.length(); ++i) {
        if (in[i] == '%' && i + 2 < in.length()) {
            int value;
            std::istringstream is(in.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                out += static_cast<char>(value);
                i += 2;
            } else {
                out += in[i];
            }
        } else if (in[i] == '+') {
            out += ' ';
        } else {
            out += in[i];
        }
    }
    return out;
}

HttpRequest HttpServer::parseRequest(const std::string& raw) {
    HttpRequest req;
    std::istringstream stream(raw);
    std::string line;

    // 1. Request Line: GET /path?query HTTP/1.1
    if (std::getline(stream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        std::istringstream lineStream(line);
        std::string version;
        lineStream >> req.method >> req.rawPath >> version;
    }

    // Parse path and query parameters
    size_t qPos = req.rawPath.find('?');
    if (qPos != std::string::npos) {
        req.path = req.rawPath.substr(0, qPos);
        std::string query = req.rawPath.substr(qPos + 1);

        size_t start = 0;
        while (start < query.length()) {
            size_t end = query.find('&', start);
            if (end == std::string::npos) end = query.length();
            std::string pair = query.substr(start, end - start);
            size_t eq = pair.find('=');
            if (eq != std::string::npos) {
                std::string k = urlDecode(pair.substr(0, eq));
                std::string v = urlDecode(pair.substr(eq + 1));
                req.queryParams[k] = v;
            }
            start = end + 1;
        }
    } else {
        req.path = req.rawPath;
    }

    // 2. Headers
    while (std::getline(stream, line) && line != "\r" && !line.empty()) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        size_t colon = line.find(':');
        if (colon != std::string::npos) {
            std::string key = line.substr(0, colon);
            std::string val = line.substr(colon + 1);
            while (!val.empty() && val.front() == ' ') val.erase(0, 1);
            req.headers[key] = val;
        }
    }

    // 3. Body
    size_t bodyPos = raw.find("\r\n\r\n");
    if (bodyPos != std::string::npos) {
        req.body = raw.substr(bodyPos + 4);
    }

    return req;
}

std::string HttpServer::serializeResponse(const HttpResponse& res) {
    std::ostringstream ss;
    ss << "HTTP/1.1 " << res.statusCode << " " << res.statusText << "\r\n";
    for (const auto& h : res.headers) {
        ss << h.first << ": " << h.second << "\r\n";
    }
    ss << "Content-Length: " << res.body.length() << "\r\n";
    ss << "Connection: close\r\n";
    ss << "\r\n";
    ss << res.body;
    return ss.str();
}

// -----------------------------------------------------------------------------
// Route Dispatcher
// -----------------------------------------------------------------------------
HttpResponse HttpServer::routeRequest(const HttpRequest& req) {
    // 1. CORS Preflight
    if (req.method == "OPTIONS") {
        return HttpResponse::corsOptions();
    }

    // 2. REST API Routes
    if (req.path == "/api/health") {
        return handleGetHealth(req);
    }
    if (req.path == "/api/products") {
        if (req.method == "GET") return handleGetProducts(req);
        if (req.method == "POST") return handlePostProduct(req);
    }
    if (req.path.rfind("/api/products/", 0) == 0) {
        int id = std::atoi(req.path.substr(14).c_str());
        return handleGetProductById(req, id);
    }
    if (req.path == "/api/orders") {
        if (req.method == "GET") return handleGetOrders(req);
        if (req.method == "POST") return handlePostOrder(req);
    }
    if (req.path.rfind("/api/orders/", 0) == 0) {
        std::string orderId = req.path.substr(12);
        return handleGetOrderById(req, orderId);
    }
    if (req.path == "/api/coupons/validate" && req.method == "POST") {
        return handleValidateCoupon(req);
    }
    if (req.path == "/api/stats" && req.method == "GET") {
        return handleGetStats(req);
    }

    // 3. Fallback to static web assets
    if (req.method == "GET") {
        return handleStaticFile(req.path);
    }

    return HttpResponse::notFound("Endpoint not found");
}

// -----------------------------------------------------------------------------
// API Handlers
// -----------------------------------------------------------------------------
HttpResponse HttpServer::handleGetHealth(const HttpRequest& req) {
    std::time_t now = std::time(nullptr);
    std::ostringstream ss;
    ss << "{"
       << "\"status\":\"ok\","
       << "\"server\":\"AuraStore C++ Winsock Backend\","
       << "\"version\":\"1.0.0\","
       << "\"timestamp\":" << now
       << "}";
    return HttpResponse::json(200, ss.str());
}

HttpResponse HttpServer::handleGetProducts(const HttpRequest& req) {
    std::string category;
    std::string search;
    double maxPrice = 0.0;
    std::string sortBy = "featured";

    auto itCat = req.queryParams.find("category");
    if (itCat != req.queryParams.end()) category = itCat->second;

    auto itSearch = req.queryParams.find("search");
    if (itSearch != req.queryParams.end()) search = itSearch->second;

    auto itMax = req.queryParams.find("maxPrice");
    if (itMax != req.queryParams.end()) {
        try { maxPrice = std::stod(itMax->second); } catch (...) {}
    }

    auto itSort = req.queryParams.find("sort");
    if (itSort != req.queryParams.end()) sortBy = itSort->second;

    std::vector<Product> products = db_.getProducts(category, search, maxPrice, sortBy);

    std::ostringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < products.size(); ++i) {
        ss << "  " << products[i].toJson();
        if (i + 1 < products.size()) ss << ",\n";
        else ss << "\n";
    }
    ss << "]";
    return HttpResponse::json(200, ss.str());
}

HttpResponse HttpServer::handleGetProductById(const HttpRequest& req, int id) {
    Product p;
    if (db_.getProductById(id, p)) {
        return HttpResponse::json(200, p.toJson());
    }
    return HttpResponse::notFound("Product not found");
}

HttpResponse HttpServer::handlePostProduct(const HttpRequest& req) {
    if (req.body.empty()) {
        return HttpResponse::badRequest("Missing JSON payload");
    }

    Product p;
    p.title = JsonHelper::extractString(req.body, "title");
    p.category = JsonHelper::extractString(req.body, "category");
    p.price = JsonHelper::extractNumber(req.body, "price");
    p.oldPrice = JsonHelper::extractNumber(req.body, "oldPrice", -1.0);
    p.image = JsonHelper::extractString(req.body, "image");
    p.description = JsonHelper::extractString(req.body, "description");
    p.badge = JsonHelper::extractString(req.body, "badge");
    p.badgeLabel = JsonHelper::extractString(req.body, "badgeLabel");
    p.rating = 5.0;
    p.reviewsCount = 1;

    if (p.title.empty() || p.price <= 0.0) {
        return HttpResponse::badRequest("Title and valid price are required");
    }

    db_.addProduct(p);
    return HttpResponse::json(201, p.toJson());
}

HttpResponse HttpServer::handleGetOrders(const HttpRequest& req) {
    std::vector<Order> orders = db_.getAllOrders();
    std::ostringstream ss;
    ss << "[\n";
    for (size_t i = 0; i < orders.size(); ++i) {
        ss << "  " << orders[i].toJson();
        if (i + 1 < orders.size()) ss << ",\n";
        else ss << "\n";
    }
    ss << "]";
    return HttpResponse::json(200, ss.str());
}

HttpResponse HttpServer::handleGetOrderById(const HttpRequest& req, const std::string& id) {
    Order ord;
    if (db_.getOrderById(id, ord)) {
        return HttpResponse::json(200, ord.toJson());
    }
    return HttpResponse::notFound("Order not found");
}

HttpResponse HttpServer::handlePostOrder(const HttpRequest& req) {
    if (req.body.empty()) {
        return HttpResponse::badRequest("Order payload required");
    }

    Order ord;
    ord.customerName = JsonHelper::extractString(req.body, "customerName");
    ord.customerEmail = JsonHelper::extractString(req.body, "customerEmail");
    ord.address = JsonHelper::extractString(req.body, "address");
    ord.city = JsonHelper::extractString(req.body, "city");
    ord.zip = JsonHelper::extractString(req.body, "zip");
    ord.couponCode = JsonHelper::extractString(req.body, "couponCode");
    ord.subtotal = JsonHelper::extractNumber(req.body, "subtotal");
    ord.discount = JsonHelper::extractNumber(req.body, "discount");
    ord.shipping = JsonHelper::extractNumber(req.body, "shipping");
    ord.total = JsonHelper::extractNumber(req.body, "total");

    if (ord.customerName.empty() || ord.customerEmail.empty()) {
        return HttpResponse::badRequest("Name and Email are required");
    }

    // Extract items array
    size_t itemsPos = req.body.find("\"items\":");
    if (itemsPos != std::string::npos) {
        size_t arrayStart = req.body.find('[', itemsPos);
        size_t arrayEnd = req.body.find(']', arrayStart);
        if (arrayStart != std::string::npos && arrayEnd != std::string::npos) {
            std::string itemsBlock = req.body.substr(arrayStart, arrayEnd - arrayStart + 1);
            size_t pos = 0;
            while ((pos = itemsBlock.find('{', pos)) != std::string::npos) {
                size_t end = itemsBlock.find('}', pos);
                if (end == std::string::npos) break;
                std::string itemJson = itemsBlock.substr(pos, end - pos + 1);
                
                OrderItem it;
                it.productId = JsonHelper::extractInt(itemJson, "productId");
                it.title = JsonHelper::extractString(itemJson, "title");
                it.price = JsonHelper::extractNumber(itemJson, "price");
                it.quantity = JsonHelper::extractInt(itemJson, "quantity", 1);
                ord.items.push_back(it);

                pos = end + 1;
            }
        }
    }

    Order savedOrder = db_.createOrder(ord);
    return HttpResponse::json(201, savedOrder.toJson());
}

HttpResponse HttpServer::handleValidateCoupon(const HttpRequest& req) {
    std::string code = JsonHelper::extractString(req.body, "code");
    if (code.empty()) {
        return HttpResponse::badRequest("Coupon code is required");
    }

    Coupon coupon;
    if (db_.validateCoupon(code, coupon)) {
        std::ostringstream ss;
        ss << "{"
           << "\"valid\":true,"
           << "\"coupon\":" << coupon.toJson()
           << "}";
        return HttpResponse::json(200, ss.str());
    }

    return HttpResponse::json(200, "{\"valid\":false, \"message\":\"Invalid or expired coupon code\"}");
}

HttpResponse HttpServer::handleGetStats(const HttpRequest& req) {
    return HttpResponse::json(200, db_.getStatsJson());
}

// -----------------------------------------------------------------------------
// Static File Serving
// -----------------------------------------------------------------------------
std::string HttpServer::getMimeType(const std::string& ext) {
    if (ext == ".html" || ext == ".htm") return "text/html; charset=utf-8";
    if (ext == ".css")  return "text/css; charset=utf-8";
    if (ext == ".js")   return "application/javascript; charset=utf-8";
    if (ext == ".json") return "application/json; charset=utf-8";
    if (ext == ".png")  return "image/png";
    if (ext == ".jpg" || ext == ".jpeg") return "image/jpeg";
    if (ext == ".svg")  return "image/svg+xml";
    if (ext == ".ico")  return "image/x-icon";
    return "text/plain; charset=utf-8";
}

HttpResponse HttpServer::handleStaticFile(const std::string& path) {
    std::string cleanPath = path;
    if (cleanPath == "/" || cleanPath.empty()) {
        cleanPath = "/index.html";
    }

    // Prevent directory traversal
    if (cleanPath.find("..") != std::string::npos) {
        return HttpResponse::notFound("Invalid path");
    }

    std::string fullPath = staticDir_ + cleanPath;
    std::ifstream file(fullPath.c_str(), std::ios::binary);
    if (!file.is_open()) {
        return HttpResponse::notFound("File not found: " + cleanPath);
    }

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Find extension
    size_t dotPos = cleanPath.rfind('.');
    std::string ext = (dotPos != std::string::npos) ? cleanPath.substr(dotPos) : "";

    return HttpResponse::file(200, getMimeType(ext), content);
}
