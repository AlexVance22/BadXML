#pragma once
#include <string>
#include <optional>
#include "color.h"


namespace xml {

bool str_is_int(const std::string& str);
bool str_is_float(const std::string& str);
bool str_is_hex_color(const std::string& str);

std::optional<int64_t> str_to_i64(const std::string& str);
std::optional<uint64_t> str_to_u64(const std::string& str);
std::optional<double> str_to_f64(const std::string& str);
std::optional<HexColor> str_to_hex_color(const std::string& str);

}
