#include "node.h"
#include <iostream>


namespace xml {

Node::Node(const Node& other)
    : name(other.name),
      ns(other.ns),
      attrs(other.attrs),
      content(other.content),
      children(other.children),
      kind(other.kind)
{}

Node::Node(Node&& other) noexcept
    : name(std::move(other.name)),
      ns(std::move(other.ns)),
      attrs(std::move(other.attrs)),
      content(std::move(other.content)),
      children(std::move(other.children)),
      kind(other.kind)
{}


Node& Node::operator=(const Node& other) {
    name = other.name;
    ns = other.ns;
    attrs = other.attrs;
    content = other.content;
    children = other.children;
    kind = other.kind;
    return *this;
}

Node& Node::operator=(Node&& other) noexcept {
    name = std::move(other.name);
    ns = std::move(other.ns);
    attrs = std::move(other.attrs);
    content = std::move(other.content);
    children = std::move(other.children);
    kind = other.kind;
    return *this;
}


Node Node::from_text(const std::string& str) {
    Node self = Node();
    self.content = str;
    self.kind = ElemKind::InnerText;
    return self;
}

Node Node::from_text(std::string&& str) {
    Node self = Node();
    self.content = std::move(str);
    self.kind = ElemKind::InnerText;
    return self;
}


const Node& Node::first_child(const std::string& child) const {
    for (auto& c: children) {
        if (c.name == child) {
            return c;
        }
    }
    throw std::exception("this child does not exist");
}

Node& Node::first_child_mut(const std::string& child) {
    for (auto& c : children) {
        if (c.name == child) {
            return c;
        }
    }
    throw std::exception("this child does not exist");
}


bool Node::operator==(const Node& rhs) const {
    if (name != rhs.name ||
        attrs != rhs.attrs ||
        content != rhs.content ||
        children.size() != rhs.children.size()) {
        return false;
    }
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] != rhs.children[i]) {
            return false;
        }
    }
    return true;
}

bool Node::operator!=(const Node& rhs) const {
    return !(*this == rhs);
}


void Node::write(std::ostream& stream) const {
    write_deep(stream, 0);
}

void Node::write_rust_style(std::ostream& stream) const {
    if (content.empty()) {
        stream << "{ name: " << name;
        stream << ", attrs: { ";
        if (!attrs.empty()) {
            const size_t size = attrs.size();
            size_t i = 0;
            for (const auto& [k, v] : attrs) {
                stream << "\"" << k << "\": \"" << v.as_string() << "\"";
                if (i < size - 1) {
                    stream << ", ";
                } else {
                    stream << " ";
                }
                i++;
            }
        }
        stream << "}";

        stream << ", children: [ ";
        if (!children.empty()) {
            for (const auto& n : children) {
                const size_t size = attrs.size();
                size_t i = 0;
                n.write_rust_style(stream);
                if (i < size - 1) {
                    stream << ", ";
                } else {
                    stream << " ";
                }
                i++;
            }
        }
        stream << "]";

        stream << " }";
    } else {
        stream << "\"" << content << "\"";
    }
}


void Node::print() const {
    write_rust_style(std::cout);
}

std::string Node::to_xml_string() const {
    return to_xml_string_deep(0);
}


void Node::write_deep(std::ostream& stream, size_t layer) const {
    for (size_t i = 0; i < layer; i++) {
        stream << '\t';
    }
    if (content.empty()) {
        stream << name;
        for (const auto& [k, v] : attrs) {
            stream << " " << k << "=" << v.as_string();
        }
        stream << '\n';

        for (const auto& n : children) {
            n.write_deep(stream, layer + 1);
        }
    } else {
        stream << content;
    }
}


std::string Node::to_xml_string_deep(size_t layer) const {
    std::string result = "";
    for (size_t i = 0; i < layer; i++) {
        result += '\t';
    }
    if (content.empty()) {
        result += "<";
        if (!ns.empty()) {
            result += ns + ":" + name;
        } else {
            result += name;
        }
        for (const auto& [k, v] : attrs) {
            result += " " + k + "=\"" + v.as_string() + "\"";
        }
        result += ">\n";

        for (const auto& n : children) {
            result += n.to_xml_string_deep(layer + 1);
        }
        if (!children.empty()) {
            for (size_t i = 0; i < layer; i++) {
                result += '\t';
            }
            result += "</" + name + ">\n";
        }
    } else {
        result += content + '\n';
    }
    return result;
}

}

std::ostream& operator<<(std::ostream& stream, const xml::Node& n) {
    n.write_rust_style(stream);
    return stream;
}

