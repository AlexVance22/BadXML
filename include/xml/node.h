#pragma once
#include "attribute.h"
#include <unordered_map>
#include <string>


namespace xml {

using AttrDict = std::unordered_map<std::string, Attribute>;

enum class ElemKind {
    Normal = 0,
    SelfClose,
    InnerText,
    ProcInstr,
};

struct Node {
    std::string name;
    std::string ns = "";
    AttrDict attrs;
    std::string content;
    std::vector<Node> children;
    ElemKind kind = ElemKind::Normal;

    Node() = default;
    Node(const Node& other);
    Node(Node&& other) noexcept;

    Node& operator=(const Node& other);
    Node& operator=(Node&& other) noexcept;

    static Node from_text(const std::string& str);
    static Node from_text(std::string&& str);
    static Node from_str(const std::string& str);

    void add_child_node(const Node& n) { children.push_back(n); }
    void add_child_node(Node&& n) { children.push_back(std::move(n)); }
    void add_child_text(const std::string& s) { children.push_back(Node::from_text(s)); }
    void add_child_text(std::string&& s) { children.push_back(Node::from_text(std::move(s))); }

    const Node& first_child() const { return children.front(); }
    Node& first_child_mut() { return children.front(); }
    const Node& first_child(const std::string& child) const;
    Node& first_child_mut(const std::string& child);

    const std::string& get_attr(const std::string& key) const { return attrs.at(key).as_string(); }
    void set_attr(const std::string& key, const std::string& val) { attrs[key] = Attribute(val); }

    bool operator==(const Node& rhs) const;
    bool operator!=(const Node& rhs) const;

    void write(std::ostream& stream) const;
    void write_rust_style(std::ostream& stream) const;
    void print() const;

    std::string to_xml_string() const;

private:

    void write_deep(std::ostream& stream, size_t layer) const;
    std::string to_xml_string_deep(size_t layer) const;
};

}

std::ostream& operator<<(std::ostream& stream, const xml::Node& n);

