#include "document.h"
#include "parsers.h"
#include <cstdio>


namespace xml {

enum class DocState {
    Free = 0,
    Name,
};

Document Document::load_from_string(const std::string& str) {
    Document doc;
    doc.root.name = "document";

    DocState state = DocState::Free;
    uint32_t line = 1;

    for (size_t i = 0; i < str.size(); i++) {
        const char c = str[i];
        if (c == '\n') { line += 1; }

        switch (state) {
        case DocState::Free:
            switch (c) {
            case '<': state = DocState::Name; break;
            default:
                if (c == '\0') {
                    return doc;
                } else if (!(c == '\r' || c == '\n' || c == '\t' || c == ' ')) {
                    throw std::exception("invalid character '{c}'in document space");
                }
            }
            break;
        case DocState::Name:
            if (('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || ('0' <= c && c <= '9') || c == '_') {
                doc.root.add_child_node(parse(str, i, line));
                state = DocState::Free;
            } else if (c == '?') {
                doc.root.add_child_node(parse_procin(str, i, line));
                state = DocState::Free;
            } else {
                throw std::exception("invalid character in tag name");
            }
            break;
        }
    }

    if (state == DocState::Free) {
        return doc;
    } else {
        throw std::exception("end of file not expected");
    }
}


Document Document::load_from_file(const std::string& filepath) {
    FILE* f = nullptr;
    fopen_s(&f, filepath.c_str(), "r");
    if (!f) { }

    fseek(f, 0, SEEK_END);
    size_t size = (size_t)ftell(f);
    fseek(f, 0, SEEK_SET);
    auto buf = std::string();
    buf.resize(size + 1, '\0');
    fread(buf.data(), sizeof(char), size, f);
    fclose(f);
    return load_from_string(buf);
}

Node& Document::get_root() {
    for (Node& child : root.children) {
        if (child.kind == ElemKind::Normal) {
            return child;
        }
    }
    throw std::exception("document does not have root node");
}

}
