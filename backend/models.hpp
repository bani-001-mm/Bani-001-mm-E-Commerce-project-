#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include "json_helper.hpp"

struct Product {
    int id = 0;
    std::string title;
    std::string category;
    double price = 0.0;
    double oldPrice = -1.0;
    double rating = 5.0;
    int reviewsCount = 0;
    std::string badge;
    std::string badgeLabel;
    std::string image;
    std::string description;
    std::vector<std::string> specs;
    bool inStock = true;

    std::string toJson() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "{"
           << "\"id\":" << id << ","
           << "\"title\":" << JsonHelper::quote(title) << ","
           << "\"category\":" << JsonHelper::quote(category) << ","
           << "\"price\":" << price << ",";
        
        if (oldPrice > 0.0) {
            ss << "\"oldPrice\":" << oldPrice << ",";
        } else {
            ss << "\"oldPrice\":null,";
        }

        ss << "\"rating\":" << rating << ","
           << "\"reviewsCount\":" << reviewsCount << ",";

        if (!badge.empty()) {
            ss << "\"badge\":" << JsonHelper::quote(badge) << ",";
            ss << "\"badgeLabel\":" << JsonHelper::quote(badgeLabel) << ",";
        } else {
            ss << "\"badge\":null,"
               << "\"badgeLabel\":\"\",";
        }

        ss << "\"image\":" << JsonHelper::quote(image) << ","
           << "\"description\":" << JsonHelper::quote(description) << ","
           << "\"specs\":[";
        
        for (size_t i = 0; i < specs.size(); ++i) {
            ss << JsonHelper::quote(specs[i]);
            if (i + 1 < specs.size()) ss << ",";
        }
        ss << "],";

        ss << "\"inStock\":" << (inStock ? "true" : "false")
           << "}";
        return ss.str();
    }
};

struct OrderItem {
    int productId = 0;
    std::string title;
    double price = 0.0;
    int quantity = 1;

    std::string toJson() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "{"
           << "\"productId\":" << productId << ","
           << "\"title\":" << JsonHelper::quote(title) << ","
           << "\"price\":" << price << ","
           << "\"quantity\":" << quantity
           << "}";
        return ss.str();
    }
};

struct Order {
    std::string orderId;
    std::string customerName;
    std::string customerEmail;
    std::string address;
    std::string city;
    std::string zip;
    std::vector<OrderItem> items;
    double subtotal = 0.0;
    std::string couponCode;
    double discount = 0.0;
    double shipping = 0.0;
    double total = 0.0;
    std::string status = "confirmed";
    std::string createdAt;

    std::string toJson() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "{"
           << "\"orderId\":" << JsonHelper::quote(orderId) << ","
           << "\"customerName\":" << JsonHelper::quote(customerName) << ","
           << "\"customerEmail\":" << JsonHelper::quote(customerEmail) << ","
           << "\"address\":" << JsonHelper::quote(address) << ","
           << "\"city\":" << JsonHelper::quote(city) << ","
           << "\"zip\":" << JsonHelper::quote(zip) << ","
           << "\"subtotal\":" << subtotal << ","
           << "\"couponCode\":" << JsonHelper::quote(couponCode) << ","
           << "\"discount\":" << discount << ","
           << "\"shipping\":" << shipping << ","
           << "\"total\":" << total << ","
           << "\"status\":" << JsonHelper::quote(status) << ","
           << "\"createdAt\":" << JsonHelper::quote(createdAt) << ","
           << "\"items\":[";

        for (size_t i = 0; i < items.size(); ++i) {
            ss << items[i].toJson();
            if (i + 1 < items.size()) ss << ",";
        }
        ss << "]}";
        return ss.str();
    }
};

struct Coupon {
    std::string code;
    std::string type; // "percentage" or "fixed"
    double value = 0.0;
    std::string description;
    int usesCount = 0;

    std::string toJson() const {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2);
        ss << "{"
           << "\"code\":" << JsonHelper::quote(code) << ","
           << "\"type\":" << JsonHelper::quote(type) << ","
           << "\"value\":" << value << ","
           << "\"description\":" << JsonHelper::quote(description) << ","
           << "\"usesCount\":" << usesCount
           << "}";
        return ss.str();
    }
};

#endif // MODELS_HPP
