#include "database.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <algorithm>
#include <direct.h> // for _mkdir on Windows

Database::Database(const std::string& dataDir)
    : dataDir_(dataDir),
      productsFilePath_(dataDir + "/products.json"),
      ordersFilePath_(dataDir + "/orders.json"),
      couponsFilePath_(dataDir + "/coupons.json") {
}

Database::~Database() {
    flushAll();
}

void Database::ensureDirectoryExists(const std::string& path) {
    _mkdir(path.c_str());
}

bool Database::initialize() {
    LockGuard lock(dbMutex_);
    ensureDirectoryExists(dataDir_);

    // 1. Load or seed products
    if (!loadProductsFromDisk()) {
        std::cout << "[Database] Initializing products table with default catalogue...\n";
        seedDefaultProducts();
        saveProductsToDisk();
    } else {
        std::cout << "[Database] Loaded " << products_.size() << " products from disk.\n";
    }

    // 2. Load or seed coupons
    if (!loadCouponsFromDisk()) {
        std::cout << "[Database] Initializing coupons table with promo codes...\n";
        seedDefaultCoupons();
        saveCouponsToDisk();
    } else {
        std::cout << "[Database] Loaded " << coupons_.size() << " coupons from disk.\n";
    }

    // 3. Load orders
    if (loadOrdersFromDisk()) {
        std::cout << "[Database] Loaded " << orders_.size() << " orders from disk.\n";
    } else {
        std::cout << "[Database] Orders table ready (0 existing orders).\n";
        saveOrdersToDisk();
    }

    return true;
}

void Database::seedDefaultProducts() {
    products_.clear();
    productIndex_.clear();

    products_ = {
        {
            1, "Aura Pro Wireless ANC Headphones", "audio",
            249.00, 299.00, 4.9, 184, "sale", "Save $50",
            "https://images.unsplash.com/photo-1505740420928-5e560c06d30e?w=800&q=80",
            "Industry-leading hybrid active noise cancellation, custom 40mm beryllium drivers, and up to 40 hours of playtime with ultra-fast USB-C quick charge.",
            {"40mm High-Resolution Drivers", "Active Hybrid Noise Cancellation", "40-Hour Battery Life", "Multipoint Bluetooth 5.3"},
            true
        },
        {
            2, "KeyPro 75% Custom Mechanical Keyboard", "keyboards",
            169.00, -1.0, 4.8, 96, "bestseller", "Best Seller",
            "https://images.unsplash.com/photo-1587829741301-dc798b83add3?w=800&q=80",
            "CNC aluminum chassis, hot-swappable tactile switches, sound-dampening silicone gasket mount, and per-key customizable South-facing RGB lighting.",
            {"CNC Anodized Aluminum Case", "Hot-Swappable PCB (3/5 Pin)", "Gasket Mounted Structure", "QMK/VIA Programmable"},
            true
        },
        {
            3, "Solid Walnut Desk Shelf & Monitor Riser", "desk",
            129.00, 149.00, 4.7, 78, "new", "New",
            "https://images.unsplash.com/photo-1527443224154-c4a3942d3acf?w=800&q=80",
            "Handcrafted from sustainably sourced solid American walnut wood with powder-coated steel legs. Elevates monitors to eye level for optimal posture.",
            {"100% Solid American Walnut", "Supports up to 60 lbs (27 kg)", "Integrated Aluminum Cable Notch", "Soft Cork Protective Feet"},
            true
        },
        {
            4, "Chronos Apex Titanium Smartwatch", "wearables",
            349.00, 399.00, 4.9, 142, "sale", "Sale",
            "https://images.unsplash.com/photo-1523275335684-37898b6baf30?w=800&q=80",
            "Grade-5 aerospace titanium casing, sapphire crystal display, continuous SpO2/ECG cardiac tracking, and 14 days battery life on a single charge.",
            {"Sapphire Crystal Glass", "Aerospace Grade 5 Titanium", "14-Day Battery Life", "5ATM Water Resistance (50m)"},
            true
        },
        {
            5, "Lumina Magnetic Desk Light Bar", "desk",
            89.00, -1.0, 4.6, 63, "", "",
            "https://images.unsplash.com/photo-1507473885765-e6ed057f782c?w=800&q=80",
            "Asymmetric optical design illuminates your desk without any screen glare. Touch controls for auto-dimming and color temperature from 2700K to 6500K.",
            {"Zero Screen Glare Optics", "CRI > 95 Color Rendering", "Wireless Desktop Rotary Dial", "USB-C Powered"},
            true
        },
        {
            6, "Orbit Wireless Earbuds with ANC", "audio",
            139.00, 179.00, 4.8, 110, "bestseller", "Popular",
            "https://images.unsplash.com/photo-1590658268037-6bf12165a8df?w=800&q=80",
            "Compact wireless earbuds with adaptive transparency mode, deep resonant bass, IPX5 water resistance, and wireless Qi charging case.",
            {"Adaptive Active Noise Cancelling", "IPX5 Sweat & Water Resistant", "32 Hours Total Playback", "Qi Wireless Charging Support"},
            true
        },
        {
            7, "Craftsman Top-Grain Leather Desk Mat", "accessories",
            59.00, -1.0, 4.7, 52, "", "",
            "https://images.unsplash.com/photo-1586953208448-b95a79798f07?w=800&q=80",
            "Full-grain vegetable-tanned leather that develops an organic patina over time. Smooth gliding surface for high-precision optical mice.",
            {"Full-Grain Vegetable Tanned Leather", "90cm x 40cm Large Surface", "Non-Slip Natural Suede Backing", "Water-Resistant Coating"},
            true
        },
        {
            8, "MagCharge 3-in-1 Foldable Charging Stand", "accessories",
            79.00, 99.00, 4.8, 88, "sale", "Save 20%",
            "https://images.unsplash.com/photo-1622445262464-84b1456045b6?w=800&q=80",
            "Charge your smartphone, smartwatch, and wireless earbuds concurrently with fast 15W Qi-certified magnetic charging in a travel-ready folding stand.",
            {"15W Fast Magnetic Wireless Charging", "Concurrently Powers 3 Devices", "Aircraft-Grade Aluminum Body", "Travel Case & 30W Adapter Included"},
            true
        },
        {
            9, "ErgoFlow Precision Wireless Vertical Mouse", "accessories",
            89.00, -1.0, 4.7, 74, "new", "New",
            "https://images.unsplash.com/photo-1615663245857-ac93bb7c39e7?w=800&q=80",
            "57-degree natural handshake angle reduces muscular forearm strain by 30%. Whisper-quiet mechanical clicks and 4000 DPI high-precision sensor.",
            {"57° Ergonomic Natural Angle", "4000 DPI High-Precision Optical Sensor", "Whisper Quiet Clicks", "Multi-Device Easy-Switch"},
            true
        },
        {
            10, "Apex Mechanical Numpad & Media Controller", "keyboards",
            75.00, 89.00, 4.6, 39, "", "",
            "https://images.unsplash.com/photo-1541140532154-b024d705b909?w=800&q=80",
            "Dedicated mechanical numeric pad with integrated knurled aluminum volume wheel and programmable macro buttons for productivity power users.",
            {"Knurled Volume/Scrub Dial", "Hot-Swappable Switches", "Bluetooth 5.1 & 2.4Ghz & Type-C", "Macro Layer Support"},
            true
        }
    };

    for (size_t i = 0; i < products_.size(); ++i) {
        productIndex_[products_[i].id] = i;
        if (products_[i].id >= nextProductId_) {
            nextProductId_ = products_[i].id + 1;
        }
    }
}

void Database::seedDefaultCoupons() {
    coupons_.clear();
    coupons_["SPRING20"] = {"SPRING20", "percentage", 20.0, "20% Spring Discount", 0};
    coupons_["AURA10"]   = {"AURA10",   "fixed",      10.0, "$10 Store Credit", 0};
    coupons_["FREESHIP"] = {"FREESHIP", "fixed",      15.0, "Free Shipping Discount", 0};
}

std::vector<Product> Database::getProducts(
    const std::string& category,
    const std::string& search,
    double maxPrice,
    const std::string& sortBy
) {
    LockGuard lock(dbMutex_);
    std::vector<Product> result;

    std::string lowerSearch = search;
    std::transform(lowerSearch.begin(), lowerSearch.end(), lowerSearch.begin(), ::tolower);

    for (const auto& p : products_) {
        // Category filter
        if (!category.empty() && category != "all" && p.category != category) {
            continue;
        }

        // Max price filter
        if (maxPrice > 0.0 && p.price > maxPrice) {
            continue;
        }

        // Search query filter
        if (!lowerSearch.empty()) {
            std::string lt = p.title;
            std::string lc = p.category;
            std::string ld = p.description;
            std::transform(lt.begin(), lt.end(), lt.begin(), ::tolower);
            std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);
            std::transform(ld.begin(), ld.end(), ld.begin(), ::tolower);

            if (lt.find(lowerSearch) == std::string::npos &&
                lc.find(lowerSearch) == std::string::npos &&
                ld.find(lowerSearch) == std::string::npos) {
                continue;
            }
        }

        result.push_back(p);
    }

    // Sort result
    if (sortBy == "price-asc") {
        std::sort(result.begin(), result.end(), [](const Product& a, const Product& b) {
            return a.price < b.price;
        });
    } else if (sortBy == "price-desc") {
        std::sort(result.begin(), result.end(), [](const Product& a, const Product& b) {
            return a.price > b.price;
        });
    } else if (sortBy == "rating") {
        std::sort(result.begin(), result.end(), [](const Product& a, const Product& b) {
            return a.rating > b.rating;
        });
    } else if (sortBy == "name-asc") {
        std::sort(result.begin(), result.end(), [](const Product& a, const Product& b) {
            return a.title < b.title;
        });
    } else { // featured (by rating * reviewsCount)
        std::sort(result.begin(), result.end(), [](const Product& a, const Product& b) {
            return (a.rating * a.reviewsCount) > (b.rating * b.reviewsCount);
        });
    }

    return result;
}

bool Database::getProductById(int id, Product& out) {
    LockGuard lock(dbMutex_);
    auto it = productIndex_.find(id);
    if (it != productIndex_.end() && it->second < products_.size()) {
        out = products_[it->second];
        return true;
    }
    return false;
}

bool Database::addProduct(Product product) {
    LockGuard lock(dbMutex_);
    product.id = nextProductId_++;
    products_.push_back(product);
    productIndex_[product.id] = products_.size() - 1;
    saveProductsToDisk();
    return true;
}

bool Database::deleteProduct(int id) {
    LockGuard lock(dbMutex_);
    auto it = productIndex_.find(id);
    if (it == productIndex_.end()) return false;

    size_t idx = it->second;
    products_.erase(products_.begin() + idx);

    // Rebuild index
    productIndex_.clear();
    for (size_t i = 0; i < products_.size(); ++i) {
        productIndex_[products_[i].id] = i;
    }
    saveProductsToDisk();
    return true;
}

Order Database::createOrder(Order order) {
    LockGuard lock(dbMutex_);

    // Generate unique order ID
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000, 99999);
    order.orderId = "#AURA-" + std::to_string(dis(gen));

    // Timestamp
    std::time_t now = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    order.createdAt = std::string(buf);
    order.status = "confirmed";

    // Track coupon usage
    if (!order.couponCode.empty()) {
        auto it = coupons_.find(order.couponCode);
        if (it != coupons_.end()) {
            it->second.usesCount++;
            saveCouponsToDisk();
        }
    }

    orders_.push_back(order);
    saveOrdersToDisk();

    std::cout << "[Database] Created order " << order.orderId 
              << " for " << order.customerName 
              << " ($" << order.total << ")\n";

    return order;
}

std::vector<Order> Database::getAllOrders() {
    LockGuard lock(dbMutex_);
    return orders_;
}

bool Database::getOrderById(const std::string& orderId, Order& out) {
    LockGuard lock(dbMutex_);
    for (const auto& ord : orders_) {
        if (ord.orderId == orderId) {
            out = ord;
            return true;
        }
    }
    return false;
}

bool Database::validateCoupon(const std::string& code, Coupon& out) {
    LockGuard lock(dbMutex_);
    std::string upperCode = code;
    std::transform(upperCode.begin(), upperCode.end(), upperCode.begin(), ::toupper);

    auto it = coupons_.find(upperCode);
    if (it != coupons_.end()) {
        out = it->second;
        return true;
    }
    return false;
}

std::string Database::getStatsJson() {
    LockGuard lock(dbMutex_);
    double totalRevenue = 0.0;
    for (const auto& o : orders_) {
        totalRevenue += o.total;
    }

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(2);
    ss << "{"
       << "\"totalProducts\":" << products_.size() << ","
       << "\"totalOrders\":" << orders_.size() << ","
       << "\"totalRevenue\":" << totalRevenue << ","
       << "\"activeCoupons\":" << coupons_.size()
       << "}";
    return ss.str();
}

void Database::flushAll() {
    LockGuard lock(dbMutex_);
    saveProductsToDisk();
    saveOrdersToDisk();
    saveCouponsToDisk();
}

// -----------------------------------------------------------------------------
// Disk Persistence Implementation (JSON serialization)
// -----------------------------------------------------------------------------
bool Database::saveProductsToDisk() {
    std::ofstream file(productsFilePath_.c_str());
    if (!file.is_open()) return false;

    file << "[\n";
    for (size_t i = 0; i < products_.size(); ++i) {
        file << "  " << products_[i].toJson();
        if (i + 1 < products_.size()) file << ",\n";
        else file << "\n";
    }
    file << "]\n";
    return true;
}

bool Database::loadProductsFromDisk() {
    std::ifstream file(productsFilePath_.c_str());
    if (!file.is_open()) return false;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (content.length() < 5) return false;

    products_.clear();
    productIndex_.clear();

    size_t pos = 0;
    while ((pos = content.find('{', pos)) != std::string::npos) {
        size_t end = content.find('}', pos);
        if (end == std::string::npos) break;

        std::string block = content.substr(pos, end - pos + 1);

        Product p;
        p.id = JsonHelper::extractInt(block, "id");
        p.title = JsonHelper::extractString(block, "title");
        p.category = JsonHelper::extractString(block, "category");
        p.price = JsonHelper::extractNumber(block, "price");
        p.oldPrice = JsonHelper::extractNumber(block, "oldPrice", -1.0);
        p.rating = JsonHelper::extractNumber(block, "rating", 5.0);
        p.reviewsCount = JsonHelper::extractInt(block, "reviewsCount");
        p.badge = JsonHelper::extractString(block, "badge");
        p.badgeLabel = JsonHelper::extractString(block, "badgeLabel");
        p.image = JsonHelper::extractString(block, "image");
        p.description = JsonHelper::extractString(block, "description");
        p.inStock = JsonHelper::extractBool(block, "inStock", true);

        if (p.id > 0) {
            products_.push_back(p);
            productIndex_[p.id] = products_.size() - 1;
            if (p.id >= nextProductId_) {
                nextProductId_ = p.id + 1;
            }
        }
        pos = end + 1;
    }

    return !products_.empty();
}

bool Database::saveOrdersToDisk() {
    std::ofstream file(ordersFilePath_.c_str());
    if (!file.is_open()) return false;

    file << "[\n";
    for (size_t i = 0; i < orders_.size(); ++i) {
        file << "  " << orders_[i].toJson();
        if (i + 1 < orders_.size()) file << ",\n";
        else file << "\n";
    }
    file << "]\n";
    return true;
}

bool Database::loadOrdersFromDisk() {
    std::ifstream file(ordersFilePath_.c_str());
    if (!file.is_open()) return false;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (content.length() < 5) return false;

    orders_.clear();
    size_t pos = 0;
    while ((pos = content.find("\"orderId\":", pos)) != std::string::npos) {
        size_t blockStart = content.rfind('{', pos);
        size_t blockEnd = content.find("]}", pos);
        if (blockEnd == std::string::npos) {
            blockEnd = content.find('}', pos);
        } else {
            blockEnd += 1;
        }

        if (blockStart == std::string::npos || blockEnd == std::string::npos) break;

        std::string block = content.substr(blockStart, blockEnd - blockStart + 1);

        Order ord;
        ord.orderId = JsonHelper::extractString(block, "orderId");
        ord.customerName = JsonHelper::extractString(block, "customerName");
        ord.customerEmail = JsonHelper::extractString(block, "customerEmail");
        ord.address = JsonHelper::extractString(block, "address");
        ord.city = JsonHelper::extractString(block, "city");
        ord.zip = JsonHelper::extractString(block, "zip");
        ord.subtotal = JsonHelper::extractNumber(block, "subtotal");
        ord.couponCode = JsonHelper::extractString(block, "couponCode");
        ord.discount = JsonHelper::extractNumber(block, "discount");
        ord.shipping = JsonHelper::extractNumber(block, "shipping");
        ord.total = JsonHelper::extractNumber(block, "total");
        ord.status = JsonHelper::extractString(block, "status");
        ord.createdAt = JsonHelper::extractString(block, "createdAt");

        if (!ord.orderId.empty()) {
            orders_.push_back(ord);
        }
        pos = blockEnd + 1;
    }
    return true;
}

bool Database::saveCouponsToDisk() {
    std::ofstream file(couponsFilePath_.c_str());
    if (!file.is_open()) return false;

    file << "[\n";
    size_t i = 0;
    for (const auto& pair : coupons_) {
        file << "  " << pair.second.toJson();
        if (++i < coupons_.size()) file << ",\n";
        else file << "\n";
    }
    file << "]\n";
    return true;
}

bool Database::loadCouponsFromDisk() {
    std::ifstream file(couponsFilePath_.c_str());
    if (!file.is_open()) return false;

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    if (content.length() < 5) return false;

    coupons_.clear();
    size_t pos = 0;
    while ((pos = content.find('{', pos)) != std::string::npos) {
        size_t end = content.find('}', pos);
        if (end == std::string::npos) break;

        std::string block = content.substr(pos, end - pos + 1);

        Coupon c;
        c.code = JsonHelper::extractString(block, "code");
        c.type = JsonHelper::extractString(block, "type");
        c.value = JsonHelper::extractNumber(block, "value");
        c.description = JsonHelper::extractString(block, "description");
        c.usesCount = JsonHelper::extractInt(block, "usesCount");

        if (!c.code.empty()) {
            coupons_[c.code] = c;
        }
        pos = end + 1;
    }

    return !coupons_.empty();
}
