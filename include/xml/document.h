#pragma once
#include "node.h"
#include <string>


namespace xml {

class Document {
private:
    Node root;

public:
    static Document load_from_string(const std::string& str);
    static Document load_from_file(const std::string& filepath);

    Node& get_root();
    Node& get_whole() { return root; }
};

}

