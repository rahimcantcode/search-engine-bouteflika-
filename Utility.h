#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>

namespace Utility {
    std::string manualToLower(const std::string& input);
    bool manualStartsWith(const std::string& input, const std::string& prefix);
    std::string manualStripPrefix(const std::string& input, const std::string& prefix);
    std::vector<std::string> manualSplit(const std::string& input, char delimiter);
}

#endif // UTILITY_H
