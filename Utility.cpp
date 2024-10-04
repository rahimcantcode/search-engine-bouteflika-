#include "Utility.h"

namespace Utility {

    // Convert a string to lowercase manually
    std::string manualToLower(const std::string& input) {
        std::string result;
        for (char c : input) {
            if (c >= 'A' && c <= 'Z') {
                result += c + ('a' - 'A');  // Convert to lowercase
            } else {
                result += c;
            }
        }
        return result;
    }

    // Check if the string starts with a given prefix manually
    bool manualStartsWith(const std::string& input, const std::string& prefix) {
        if (prefix.size() > input.size()) return false;
        for (size_t i = 0; i < prefix.size(); ++i) {
            if (input[i] != prefix[i]) {
                return false;
            }
        }
        return true;
    }

    // Remove a prefix from a string manually, without using substr
    std::string manualStripPrefix(const std::string& input, const std::string& prefix) {
        if (!manualStartsWith(input, prefix)) {
            return input;  // Return original if prefix not found
        }
        std::string result;
        for (size_t i = prefix.size(); i < input.size(); ++i) {
            result += input[i];
        }
        return result;
    }

    // Split a string by a delimiter manually
    std::vector<std::string> manualSplit(const std::string& input, char delimiter) {
        std::vector<std::string> result;
        std::string current;
        for (char c : input) {
            if (c == delimiter) {
                if (!current.empty()) {
                    result.push_back(current);
                    current.clear();
                }
            } else {
                current += c;
            }
        }
        if (!current.empty()) {
            result.push_back(current);
        }
        return result;
    }
}
