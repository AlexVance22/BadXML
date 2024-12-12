#include "casts.h"


namespace xml
{

bool str_is_int(const std::string& str) {
    for (char c : str) {
        if (!std::isdigit(c)) {
            return false;
        }
    }

    return true;
}

bool str_is_float(const std::string& str) {
    bool haspoint = false;

    for (char c : str) {
        if (!std::isdigit(c)) {
            if (c != '.' || haspoint) {
                return false;
            } else {
                haspoint = true;
            }
        }
    }

    return true;
}

bool str_is_hex_color(const std::string& str) {
    if (str[0] != '#' || !(str.size() == 4 || str.size() == 7)) {
        return false;
    }

    for (size_t i = 1; i < str.size(); i++) {
        if (!std::isxdigit(str[i])) {
            return false;
        }
    }

    return true;
}


std::optional<int64_t> str_to_i64(const std::string& str) {
    if (str_is_int(str)) {
        return std::stoll(str);
    } else {
        return {};
    }
}

std::optional<uint64_t> str_to_u64(const std::string& str) {
    if (str_is_int(str)) {
        return std::stoull(str);
    } else {
        return {};
    }
}

std::optional<double> str_to_f64(const std::string& str) {
    if (str_is_float(str)) {
        return std::stod(str);
    } else {
        return {};
    }
}


static uint8_t from_hex_digit(char d) {
    if ('0' <= d && d <= '9') {
        return d - '0';
    } else if ('a' <= d && d <= 'f') {
        return d - 'a' + 10;
    } else if ('A' <= d && d <= 'F') {
        return d - 'A' + 10;
    } else {
        return 0;
    }
}

std::optional<HexColor> str_to_hex_color(const std::string& str) {
    if (str_is_hex_color(str)) {
        HexColor col;
        if (str.size() == 4) {
            const uint8_t r1 = from_hex_digit(str[1]);
            const uint8_t g1 = from_hex_digit(str[2]);
            const uint8_t b1 = from_hex_digit(str[3]);
            const uint8_t r = r1 * 16 + r1;
            const uint8_t g = g1 * 16 + g1;
            const uint8_t b = b1 * 16 + b1;
            return HexColor{ r, g, b, 255 };
        } else {
            const uint8_t r1 = from_hex_digit(str[1]);
            const uint8_t r2 = from_hex_digit(str[2]);
            const uint8_t g1 = from_hex_digit(str[3]);
            const uint8_t g2 = from_hex_digit(str[4]);
            const uint8_t b1 = from_hex_digit(str[5]);
            const uint8_t b2 = from_hex_digit(str[6]);
            const uint8_t r = r1 * 16 + r2;
            const uint8_t g = g1 * 16 + g2;
            const uint8_t b = b1 * 16 + b2;
            return HexColor{ r, g, b, 255 };
            return col;
        }
    } else {
        return {};
    }
}

}
