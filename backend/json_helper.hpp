#ifndef JSON_HELPER_HPP
#define JSON_HELPER_HPP

#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>
#include <cctype>

namespace JsonHelper {

inline std::string escape(const std::string& s) {
    std::ostringstream o;
    for (char c : s) {
        switch (c) {
            case '"':  o << "\\\""; break;
            case '\\': o << "\\\\"; break;
            case '\b': o << "\\b";  break;
            case '\f': o << "\\f";  break;
            case '\n': o << "\\n";  break;
            case '\r': o << "\\r";  break;
            case '\t': o << "\\t";  break;
            default:
                if (static_cast<unsigned char>(c) <= 0x1f) {
                    o << "\\u00" << std::hex << (int)c;
                } else {
                    o << c;
                }
        }
    }
    return o.str();
}

inline std::string quote(const std::string& s) {
    return "\"" + escape(s) + "\"";
}

// Simple JSON extraction helpers for incoming POST payloads
inline std::string extractString(const std::string& json, const std::string& key) {
    // Try with quotes first
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) {
        // Try without quotes
        pattern = key + ":";
        pos = json.find(pattern);
        if (pos == std::string::npos) return "";
        pos += pattern.length();
    } else {
        pos = json.find(':', pos + pattern.length());
        if (pos == std::string::npos) return "";
        pos++;
    }

    // Skip whitespace
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\r' || json[pos] == '\n')) {
        pos++;
    }

    if (pos >= json.length()) return "";

    if (json[pos] == '"') {
        pos++; // Skip opening quote
        std::string result;
        bool inEscape = false;
        for (size_t i = pos; i < json.length(); ++i) {
            char c = json[i];
            if (inEscape) {
                result += c;
                inEscape = false;
            } else if (c == '\\') {
                inEscape = true;
            } else if (c == '"') {
                break;
            } else {
                result += c;
            }
        }
        return result;
    } else {
        // Unquoted value (up to comma, closing brace, or whitespace)
        size_t end = pos;
        while (end < json.length() && json[end] != ',' && json[end] != '}' && json[end] != ' ' && json[end] != '\r' && json[end] != '\n') {
            end++;
        }
        return json.substr(pos, end - pos);
    }
}

inline double extractNumber(const std::string& json, const std::string& key, double defaultValue = 0.0) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) {
        pattern = key + ":";
        pos = json.find(pattern);
        if (pos == std::string::npos) return defaultValue;
        pos += pattern.length();
    } else {
        pos = json.find(':', pos + pattern.length());
        if (pos == std::string::npos) return defaultValue;
        pos++;
    }

    // skip spaces
    while (pos < json.length() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\r' || json[pos] == '\n')) {
        pos++;
    }

    size_t end = pos;
    while (end < json.length() && (std::isdigit(json[end]) || json[end] == '.' || json[end] == '-')) {
        end++;
    }

    if (end > pos) {
        try {
            return std::stod(json.substr(pos, end - pos));
        } catch (...) {
            return defaultValue;
        }
    }
    return defaultValue;
}

inline int extractInt(const std::string& json, const std::string& key, int defaultValue = 0) {
    return static_cast<int>(extractNumber(json, key, defaultValue));
}

inline bool extractBool(const std::string& json, const std::string& key, bool defaultValue = false) {
    std::string pattern = "\"" + key + "\"";
    size_t pos = json.find(pattern);
    if (pos == std::string::npos) {
        pattern = key + ":";
        pos = json.find(pattern);
        if (pos == std::string::npos) return defaultValue;
        pos += pattern.length();
    } else {
        pos = json.find(':', pos + pattern.length());
        if (pos == std::string::npos) return defaultValue;
        pos++;
    }

    std::string sub = json.substr(pos, 10);
    if (sub.find("true") != std::string::npos) return true;
    if (sub.find("false") != std::string::npos) return false;
    return defaultValue;
}

} // namespace JsonHelper

#endif // JSON_HELPER_HPP
