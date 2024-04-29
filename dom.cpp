#include <cstddef>
#include <iostream>
#include <map>
#include <vector>

struct ElementData {
    std::string tag_name;
    std::map<std::string, std::string> attributes;
};

struct Node {
    std::vector<Node> children;
    std::variant<ElementData, std::string> node_type;
};

Node createTextNode(const std::string data) {
    return {.children = {}, .node_type = data};
}

Node createElementNode(std::string name,
                       std::map<std::string, std::string> attributes,
                       std::vector<Node> children) {
    ElementData data{.tag_name = name, .attributes = attributes};
    return {.children = children, .node_type = data};
}

void indent(size_t depth) {
    std::cout << '\n';
    for (size_t i = 0; i < depth; i++)
        std::cout << "    ";
}

void printDom(const Node &root, size_t depth = 0) {
    if (depth > 0)
        indent(depth);

    if (std::holds_alternative<std::string>(root.node_type)) {
        std::cout << "\"" << std::get<std::string>(root.node_type) << "\"";
        return;
    }

    std::cout << "<" << std::get<ElementData>(root.node_type).tag_name;
    for (const auto &[key, value] :
         std::get<ElementData>(root.node_type).attributes) {
        std::cout << ' ' << key << "=\"" << value << "\"";
    }
    std::cout << ">";

    for (Node child : root.children) {
        printDom(child, depth + 1);
    }

    if (!root.children.empty())
        indent(depth);
    std::cout << "</" << std::get<ElementData>(root.node_type).tag_name << ">";
}

int main() {
    Node elem{createElementNode(
        "DIV", {},
        {
            createElementNode("P", {{"id", "subtitle"}, {"class", "pink"}}, {}),
            createElementNode(
                "P", {},
                {createTextNode("hello"),
                 createElementNode("A", {}, {createTextNode("click me")}),
                 createElementNode(
                     "SPAN", {},
                     {createTextNode("something's in the water")})}),
            createElementNode("P", {}, {}),
        })};
    printDom(elem);
}
