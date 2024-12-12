#include "str_util.h"


std::string trim_whitespace(const std::string& str) {
    int i = 0;
    for (; i < (int)str.size() && (str[i] == '\r' || str[i] == '\n' || str[i] == '\t' || str[i] == ' '); i++) {
    }
    if (i == (int)str.size()) {
        return "";
    }

    int j = (int)str.size() - 1;
    for (; j >= 0 && (str[j] == '\r' || str[j] == '\n' || str[j] == '\t' || str[j] == ' '); j--) {
    }
    return str.substr(i, j - i + 1);
}

std::string to_lowercase(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.size(); i++) {
        if ('A' <= result[i] && result[i] <= 'Z') {
            result[i] += 32;
        }
    }
    return result;
}

std::string to_uppercase(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.size(); i++) {
        if ('a' <= result[i] && result[i] <= 'a') {
            result[i] -= 32;
        }
    }
    return result;
}


bool starts_with(const std::string& str, const std::string& pat) {
    if (pat.size() > str.size()) {
        return false;
    } else {
        for (size_t i = 0; i < pat.size(); i++) {
            if (str[i] != pat[i]) {
                return false;
            }
        }
        return true;
    }
}

bool ends_with(const std::string& str, const std::string& pat) {
    if (pat.size() > str.size()) {
        return false;
    } else {
        for (size_t i = str.size() - 1 - pat.size(); i < str.size(); i++) {
            if (str[i] != pat[i]) {
                return false;
            }
        }
        return true;
    }
}

bool contains(const std::string& str, const std::string& pat) {
    if (pat.size() > str.size()) {
        return false;
    } else {
        for (size_t i = 0; i < str.size() - pat.size(); i++) {
            if (str[i] == pat[i]) {
                bool found = true;
                for (size_t j = 0; j < pat.size(); j++) {
                    if (str[i+j] != pat[i+j]) {
                        found = false;
                        break;
                    }
                }
                if (found) {
                    return true;
                }
            }
        }
        return false;
    }
}

