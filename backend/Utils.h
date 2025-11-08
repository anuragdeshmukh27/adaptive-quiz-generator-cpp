#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <cctype> // For std::tolower

/**
 * @brief We mark these functions as 'inline' to tell the linker
 * that it's okay for this code to be included in multiple .cpp files.
 * This solves the "multiple definition" error.
 */

// Trims whitespace from the beginning and end of a string
inline std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (std::string::npos == first) {
        return str;
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Converts a string to lowercase
inline std::string toLower(std::string s) {
    for (char& c : s) {
        c = std::tolower(c);
    }
    return s;
}

#endif // UTILS_H

