#ifndef DATABASE_HPP
#define DATABASE_HPP

#include <string>
#include <vector>
#include <map>
#include "models.hpp"
#include "win_sync.hpp"

class Database {
public:
    explicit Database(const std::string& dataDir = "data");
    ~Database();

    // Initialization & seeding
    bool initialize();

    // Product Queries & Mutations
    std::vector<Product> getProducts(
        const std::string& category = "",
        const std::string& search = "",
        double maxPrice = 0.0,
        const std::string& sortBy = "featured"
    );
    bool getProductById(int id, Product& out);
    bool addProduct(Product product);
    bool deleteProduct(int id);

    // Orders Management
    Order createOrder(Order order);
    std::vector<Order> getAllOrders();
    bool getOrderById(const std::string& orderId, Order& out);

    // Coupon Queries
    bool validateCoupon(const std::string& code, Coupon& out);

    // Analytics / Metrics
    std::string getStatsJson();

    // Explicit disk sync
    void flushAll();

private:
    std::string dataDir_;
    std::string productsFilePath_;
    std::string ordersFilePath_;
    std::string couponsFilePath_;

    mutable Mutex dbMutex_;

    std::vector<Product> products_;
    std::map<int, size_t> productIndex_;

    std::vector<Order> orders_;
    std::map<std::string, Coupon> coupons_;

    int nextProductId_ = 11;

    // Helpers
    void seedDefaultProducts();
    void seedDefaultCoupons();
    bool loadProductsFromDisk();
    bool loadOrdersFromDisk();
    bool loadCouponsFromDisk();
    bool saveProductsToDisk();
    bool saveOrdersToDisk();
    bool saveCouponsToDisk();

    void ensureDirectoryExists(const std::string& path);
};

#endif // DATABASE_HPP
