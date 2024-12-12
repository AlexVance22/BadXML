#include "parsers.h"
#include "str_util.h"
#include <exception>


namespace xml {

enum class State {
    Free = 0,
    Name,
    Open1,
    Open1NoNS,
    Open2,
    OpenClose,
    Close1,
    Close2,
    Close2NoNS,
    CloseEnd,
};


Node Node::from_str(const std::string& src) {
    uint32_t line = 1;
    State state = State::Free;

    for (size_t i = 0; i < src.size(); i++) {
        const char c = src[i];
        if (c == '\n') { line += 1; }

        switch (state) {
        case State::Free:
            switch (c) {
            case '<': state = State::Name; break;
            default:
                if (!(c == '\r' || c == '\n' || c == '\t' || c == ' ')) {
                   throw std::exception("expected root node");
                }
            }
            break;
        case State::Name:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_') {
                return parse(src, i, line);
            } else if (c == '?') {
                return parse_procin(src, i, line);
            } else {
                throw std::exception("invalid character in tag name");
            }
            break;
        default:
            break;
        }
    }

    throw std::exception("end of file not expected");
}


Node parse(const std::string& src, size_t& i, uint32_t& line) {
    auto current = Node();
    auto inner_text = std::string();
    auto close = std::string();
    auto close_ns = std::string();
    auto state = State::Open1;

    for (; i < src.size(); i++) {
        char c = src[i];
        if (c == '\n') { line++; }

        switch (state) {
        case State::Free:
            if (c == '<') {
                auto trimmed = trim_whitespace(inner_text);
                if (!trimmed.empty()) {
                    current.children.push_back(Node::from_text(trimmed));
                    inner_text.clear();
                }
                state = State::Name;
            } else {
                inner_text.push_back(c);
            }
            break;
        case State::Name:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_') {
                current.children.push_back(parse(src, i, line));
                state = State::Free;
            } else if (c == '!') {
                parse_comment(src, i, line);
                state = State::Free;
            } else if (c == '?') {
                current.children.push_back(parse_procin(src, i, line));
                state = State::Free;
            } else if (c == '/') {
                state = State::Close1;
            } else {
                throw std::exception("invalid character in tag name");
            }
            break;
        case State::Open1:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_' || c == '-' || c == '.') {
                current.name.push_back(c);
            } else if (c == ':') {
                current.ns = current.name;
                current.name = "";
                state = State::Open1NoNS;
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                state = State::Open2;
            } else if (c == '/') {
                state = State::OpenClose;
            } else if (c == '>') {
                state = State::Free;
            } else {
                throw std::exception("invalid character in tag name");
            }
            break;
        case State::Open1NoNS:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_' || c == '-' || c == '.') {
                current.name.push_back(c);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                state = State::Open2;
            } else if (c == '/') {
                state = State::OpenClose;
            } else if (c == '>') {
                state = State::Free;
            } else {
                throw std::exception("invalid character in tag name");
            }
            break;
        case State::Open2:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_') {
                auto [key, val] = parse_attribute(src, i, line);
                current.set_attr(key, val);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                // wait
            } else if (c == '/') {
                state = State::OpenClose;
            } else if (c == '>') {
                state = State::Free;
            } else {
                throw std::exception("invalid character in attribute name");
            }
            break;
        case State::OpenClose:
            if (c == '>') {
                current.kind = ElemKind::SelfClose;
                return current;
            } else {
                throw std::exception("self-closing tag not closed");
            }
            break;
        case State::Close1:
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
                close.push_back(c);
                state = State::Close2;
            } else {
                throw std::exception("invalid character in closing tag name");
            }
            break;
        case State::Close2:
            if (c == '>') {
                if (close == current.name) {
                    return current;
                } else {
                    throw std::exception("closing tag does not match opening tag");
                }
            } else if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_' || c == '-' || c == '.') {
                close.push_back(c);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                state = State::CloseEnd;
            } else if (c == ':') {
                close_ns = close;
                close = "";
                state = State::Close2NoNS;
            } else {
                throw std::exception("invalid character in closing tag name");
            }
            break;
        case State::Close2NoNS:
            if (c == '>') {
                if (close == current.name) {
                    return current;
                } else {
                    throw std::exception("closing tag does not match opening tag");
                }
            } else if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_' || c == '-' || c == '.') {
                close.push_back(c);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                state = State::CloseEnd;
            } else {
                throw std::exception("invalid character in closing tag name");
            }
            break;
        case State::CloseEnd:
            if (c == '>') {
                if (close == current.name && close_ns == current.ns) {
                    return current;
                } else {
                    throw std::exception("closing tag does not match opening tag");
                }
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                // wait
            } else {
                throw std::exception("closing tag not closed");
            }
            break;
        }
    }

    throw std::exception("end of file not expected");
}


enum class AttrParseState {
    Key,
    Eq,
    Quote,
    Value,
    Escape,
};

std::pair<std::string, std::string> parse_attribute(const std::string& src, size_t& i, uint32_t& line) {
    auto key = std::string();
    auto val = std::string();
    auto state = AttrParseState::Key;

    for (; i < src.size(); i++) {
        char c = src[i];
        if (c == '\n') { line++; }

        switch (state) {
        case AttrParseState::Key:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_' || c == '-' || c == ':') {
                key.push_back(c);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                state = AttrParseState::Eq;
            } else if (c == '=') {
                state = AttrParseState::Quote;
            } else {
                throw std::exception("invalid character in attribute name");
            }
            break;
        case AttrParseState::Eq:
            if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                // wait
            } else if (c == '=') {
                state = AttrParseState::Quote;
            } else {
                throw std::exception("expected '=' after attribute name");
            }
            break;
        case AttrParseState::Quote:
            if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                // wait
            } else if (c == '\"') {
                state = AttrParseState::Value;
            } else {
                throw std::exception("expected '\"' after attribute definition");
            }
            break;
        case AttrParseState::Value:
            if (c == '\"') {
                return { key, val };
            } else if (c == '\\') {
                state = AttrParseState::Escape; break;
            } else {
                val.push_back(c);
            }
            break;
        case AttrParseState::Escape:
            switch (c) {
            case 'n':  val.push_back('\n'); break;
            case 'r':  val.push_back('\r'); break;
            case 't':  val.push_back('\t'); break;
            case '\"': val.push_back('\"'); break;
            case '\\': val.push_back('\\'); break;
            default: throw std::exception("invalid escape character");
            }
            state = AttrParseState::Value;
            break;
        }
    }

    throw std::exception("end of file not expected in attribute value");
}

Node parse_procin(const std::string& src, size_t& i, uint32_t& line) {
    auto current = Node();
    current.kind = ElemKind::ProcInstr;
    auto state = State::Open1;
    i++;

    for (; i < src.size(); i++) {
        char c = src[i];
        if (c == '\n') { line++; }

        switch (state) {
        case State::Open1:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_' || c == '-' || c == '.') {
                current.name.push_back(c);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                state = State::Open2;
            } else {
                throw std::exception("invalid character in tag name");
            }
            break;
        case State::Open2:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_') {
                auto [key, val] = parse_attribute(src, i, line);
                current.set_attr(key, val);
            } else if (c == '\t' || c == '\r' || c == '\n' || c == ' ') {
                // wait
            } else if (c == '?') {
                state = State::OpenClose;
            } else {
                throw std::exception("invalid character in attribute name");
            }
            break;
        case State::OpenClose:
            if (c == '>') {
                return current;
            } else {
                throw std::exception("self-closing tag not closed");
            }
            break;
        default:
            break;
        }
    }

    throw std::exception("end of file not expected");
}

void parse_comment(const std::string& src, size_t& i, uint32_t& line) {
    uint32_t state = 0;

    for (; i < src.size(); i++) {
        char c = src[i];
        if (c == '\n') { line++; }

        switch (state) {
        case 0: case 1:
            if (c == '-') {
                state++;
            } else {
                throw std::exception("invalid character in comment prelude");
            }
            break;
        case 2:
            if (c == '-') {
                state++;
            }
            break;
        case 3:
            if (c == '-') {
                state++;
            } else {
                state--;
            }
            break;
        case 4:
            if (c == '>') {
                return;
            } else {
                throw std::exception("comment not terminated after '--' string");
            }
            break;
        }
    }
}

}

