#pragma once
#include <string>
#include "node.h"


namespace xml {

Node parse(const std::string& src, size_t& i, uint32_t& line);

std::pair<std::string, std::string> parse_attribute(const std::string& src, size_t& i, uint32_t& line);
Node parse_procin(const std::string& src, size_t& i, uint32_t& line);
void parse_comment(const std::string& src, size_t& i, uint32_t& line);

}

