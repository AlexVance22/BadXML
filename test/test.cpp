#include "mscmptest/asserts.h"

#include "document.h"


void parse_single_elem();
void parse_attributes();
void parse_children();
void parse_interleave();
void parse_prelude();
void parse_expect_fail0();
void parse_expect_fail1();
void parse_expect_fail2();
void parse_real();
void parse_final_boss();


int main() {
    test(parse_single_elem);
    test(parse_attributes);
    test(parse_children);
    test(parse_interleave);
    test(parse_prelude);
    test(parse_expect_fail0);
    test(parse_expect_fail1);
    test(parse_expect_fail2);
    test(parse_real);
    test(parse_final_boss);
}


void parse_single_elem() {
    auto doc = xml::Document::load_from_file("test/res/test0.xml");

    xml::Node node;
    node.name = "document";
    auto& svg = node.children.emplace_back();
    svg.name = "svg";

    assert_eq(node, doc.get_whole());
}

void parse_attributes() {
    auto doc = xml::Document::load_from_file("test/res/test1.xml");

    xml::Node node;
    node.name = "document";
    auto& svg = node.children.emplace_back();
    svg.name = "svg";
    svg.set_attr("version", "1.1");
    svg.set_attr("width",   "624.000000");
    svg.set_attr("height",  "224.000000");
    svg.set_attr("xmlns",   "http://www.w3.org/2000/svg");

    assert_eq(node, doc.get_whole());
}

void parse_children() {
    auto doc = xml::Document::load_from_file("test/res/test2.xml");

    xml::Node node;
    node.name = "document";
    auto& svg = node.children.emplace_back();
    svg.name = "svg";
    svg.set_attr("version", "1.1");
    svg.set_attr("width",   "624.000000");
    svg.set_attr("height",  "224.000000");
    svg.set_attr("xmlns",   "http://www.w3.org/2000/svg");

    auto& circle = svg.children.emplace_back();
    circle.name = "circle";
    circle.set_attr("cx",           "312.000000");
    circle.set_attr("cy",           "162.000000");
    circle.set_attr("r",            "42");
    circle.set_attr("stroke",       "black");
    circle.set_attr("stroke-width", "1.5");
    circle.set_attr("fill",         "white");

    auto& text = svg.children.emplace_back();
    text.name = "text";
    text.set_attr("x",           "312.000000");
    text.set_attr("y",           "172.000000");
    text.set_attr("font-family", "CMU Serif");
    text.set_attr("font-size",   "26");
    text.set_attr("font-style",  "italic");
    text.set_attr("text-anchor", "middle");
    text.set_attr("fill",        "black");
    text.add_child_text("v', 100");

    assert_eq(node, doc.get_whole());
}

void parse_interleave() {
    auto doc = xml::Document::load_from_file("test/res/test3.xml");

    xml::Node node;
    node.name = "document";
    auto& html = node.children.emplace_back();
    html.name = "html";

    auto& div = html.children.emplace_back();
    div.name = "div";
    div.set_attr("class", "main");

    div.add_child_text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce a elementum enim. Cras vel risus non sem commodo volutpat");

    auto& strong = div.children.emplace_back();
    strong.name = "strong";
    strong.add_child_text("at ac quam. Aenean scelerisque elit eget pharetra aliquam. Donec lobortis posuere tortor, sit amet dictum risus lobortis");

    div.add_child_text("nec. Proin vitae felis dapibus, eleifend nunc nec, scelerisque quam. Curabitur sed blandit nisi. Nullam vulputate sem");

    auto& ol = div.children.emplace_back();
    ol.name = "ol";

    auto& li1 = ol.children.emplace_back();
    li1.name = "li";
    li1.add_child_text("nulla, ut efficitur urna fringilla ut. Aenean cursus fringilla dui eget feugiat. Nunc a odio lacus. Pellentesque viverra");

    auto& li2 = ol.children.emplace_back();
    li2.name = "li";
    li2.add_child_text("ut elit nec ultricies. Nulla vel orci ligula. Morbi commodo nulla in neque mollis, eget facilisis nisi pulvinar. Quisque");

    auto& li3 = ol.children.emplace_back();
    li3.name = "li";
    li3.add_child_text("luctus fringilla purus et viverra.");

    assert_eq(node, doc.get_whole());
}

void parse_prelude() {
    auto doc = xml::Document::load_from_file("test/res/test4.xml");

    xml::Node node;
    node.name = "document";

    auto& prelude = node.children.emplace_back();
    prelude.name = "xml";
    prelude.set_attr("version", "1.0");
    prelude.set_attr("encoding", "UTF-8");

    auto& xml = node.children.emplace_back();
    xml.name = "xml";

    auto& div = xml.children.emplace_back();
    div.name = "div";
    div.set_attr("class", "main");

    div.add_child_text("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Fusce a elementum enim. Cras vel risus non sem commodo volutpat");

    auto& strong = div.children.emplace_back();
    strong.name = "strong";
    strong.add_child_text("at ac quam. Aenean scelerisque elit eget pharetra aliquam. Donec lobortis posuere tortor, sit amet dictum risus lobortis");

    assert_eq(node, doc.get_whole());
}

void parse_expect_fail0() {
    assert_throws(xml::Document::load_from_file("test/res/test_fail0.xml"), std::exception);
}

void parse_expect_fail1() {
    assert_throws(xml::Document::load_from_file("test/res/test_fail1.xml"), std::exception);
}

void parse_expect_fail2() {
    assert_throws(xml::Document::load_from_file("test/res/test_fail2.xml"), std::exception);
}

void parse_real() {
    auto doc = xml::Document::load_from_file("test/res/github.svg");
}

void parse_final_boss() {
    auto doc = xml::Document::load_from_file("test/res/final_boss.svg");
}
