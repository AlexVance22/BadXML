#include "attribute.h"
#include "casts.h"


namespace xml {

std::optional<int64_t> Attribute::as_i64() const {
    return str_to_i64(m_val);
}

std::optional<uint64_t> Attribute::as_u64() const {
    return str_to_u64(m_val);
}

std::optional<double> Attribute::as_f64() const {
    return str_to_f64(m_val);
}

std::optional<HexColor> Attribute::as_hex_color() const {
    return str_to_hex_color(m_val);
}

}
