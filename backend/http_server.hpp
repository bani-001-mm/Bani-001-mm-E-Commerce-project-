#ifndef HTTP_SERVER_HPP
#define HTTP_SERVER_HPP

#include <string>
#include <map>
#include <memory>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "database.hpp"

struct HttpRequest {
    std::string method;
    std::string rawPath;
    std::string path;
    std::map<std::string, std::string> queryParams;
    std::map<std::string, std::string> headers;
    std::string body;
};

struct HttpResponse {
    int statusCode = 200;
    std::string statusText = "OK";
    std::map<std::string, std::string> headers;
    std::string body;

    static HttpResponse json(int code, const std::string& jsonBody);
    static HttpResponse text(int code, const std::string& textBody);
    static HttpResponse file(int code, const std::string& mimeType, const std::string& data);
    static HttpResponse corsOptions();
    static HttpResponse notFound(const std::string& msg = "Not Found");
    static HttpResponse badRequest(const std::string& msg = "Bad Request");
};

class HttpServer {
public:
    HttpServer(int port, Database& db, const std::string& staticDir = "..");
    ~HttpServer();

    bool start();
    void stop();
    void handleClient(SOCKET clientSocket);

private:
    int port_;
    Database& db_;
    std::string staticDir_;
    bool running_;
    SOCKET listenSocket_;

    HttpRequest parseRequest(const std::string& raw);
    std::string serializeResponse(const HttpResponse& res);

    HttpResponse routeRequest(const HttpRequest& req);

    // API Handlers
    HttpResponse handleGetHealth(const HttpRequest& req);
    HttpResponse handleGetProducts(const HttpRequest& req);
    HttpResponse handleGetProductById(const HttpRequest& req, int id);
    HttpResponse handlePostProduct(const HttpRequest& req);
    HttpResponse handleGetOrders(const HttpRequest& req);
    HttpResponse handleGetOrderById(const HttpRequest& req, const std::string& id);
    HttpResponse handlePostOrder(const HttpRequest& req);
    HttpResponse handleValidateCoupon(const HttpRequest& req);
    HttpResponse handleGetStats(const HttpRequest& req);

    // Static Assets
    HttpResponse handleStaticFile(const std::string& path);
    std::string getMimeType(const std::string& extension);
    std::string urlDecode(const std::string& in);
};

#endif // HTTP_SERVER_HPP
