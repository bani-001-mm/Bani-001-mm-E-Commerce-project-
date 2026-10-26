#include <iostream>
#include <string>
#include <csignal>
#include <cstdlib>
#include "database.hpp"
#include "http_server.hpp"

HttpServer* g_server = nullptr;

void signalHandler(int signum) {
    std::cout << "\n[AuraStore] Interrupted signal (" << signum << ") received. Shutting down...\n";
    if (g_server) {
        g_server->stop();
    }
    exit(signum);
}

int main(int argc, char* argv[]) {
    int port = 8080;
    if (argc > 1) {
        try {
            port = std::stoi(argv[1]);
        } catch (...) {
            std::cerr << "Invalid port supplied. Falling back to 8080.\n";
            port = 8080;
        }
    }

    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    std::cout << "\n============================================================\n";
    std::cout << "          A U R A S T O R E   C + +   B A C K E N D         \n";
    std::cout << "============================================================\n";

    // 1. Initialize persistent database
    std::cout << "[Step 1/2] Initializing C++ Database Engine...\n";
    Database db("data");
    if (!db.initialize()) {
        std::cerr << "[Error] Failed to initialize Database engine!\n";
        return 1;
    }

    // 2. Start HTTP server
    std::cout << "[Step 2/2] Starting HTTP Server on port " << port << "...\n";
    HttpServer server(port, db, "..");
    g_server = &server;

    std::cout << "\nAvailable REST API Endpoints:\n";
    std::cout << "  • GET  /api/health            - Health check & server status\n";
    std::cout << "  • GET  /api/products          - List products (supports ?category= &search= &maxPrice= &sort=)\n";
    std::cout << "  • GET  /api/products/:id      - Get single product by ID\n";
    std::cout << "  • POST /api/products          - Add new product to database\n";
    std::cout << "  • GET  /api/orders            - View all customer orders\n";
    std::cout << "  • POST /api/orders            - Submit customer order & receive receipt\n";
    std::cout << "  • GET  /api/orders/:id        - Look up specific order\n";
    std::cout << "  • POST /api/coupons/validate  - Validate promo code\n";
    std::cout << "  • GET  /api/stats             - Summary sales metrics\n";
    std::cout << "  • GET  /                      - Serves frontend index.html\n\n";

    if (!server.start()) {
        std::cerr << "[Error] Server could not be started.\n";
        return 1;
    }

    return 0;
}
