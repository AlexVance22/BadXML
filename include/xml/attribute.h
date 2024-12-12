#pragma once
#include <optional>
#include <string>
#include "color.h"


namespace xml {

class Attribute {
private:
    std::string m_val;

public:
    Attribute() = default;
    Attribute(const std::string& val) : m_val(val) {}
    Attribute(std::string&& val) noexcept : m_val(std::move(val)) {}

    const std::string& as_string() const { return m_val; }
    std::optional<int64_t> as_i64() const;
    std::optional<uint64_t> as_u64() const;
    std::optional<double> as_f64() const;
    std::optional<HexColor> as_hex_color() const;

    void set(const std::string& val) { m_val = val; }
    void set(std::string&& val) { m_val = std::move(val); }

    bool operator==(const Attribute& rhs) const { return m_val == rhs.m_val; }
    bool operator!=(const Attribute& rhs) const { return !(*this == rhs); }
};

}

