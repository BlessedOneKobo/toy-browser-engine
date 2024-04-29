#include <iostream>
#include <map>
#include <variant>
#include <vector>

typedef std::map<std::string, std::string> AttrMap;

struct ElementData {
    std::string tag_name;
    AttrMap attributes;
};

struct DocumentType {
    std::string name;
    std::string publicId;
    std::string systemId;
};

typedef std::variant<std::string, ElementData, DocumentType> NodeType;

struct Node {
    NodeType node_type;
    std::vector<Node> children;
};

Node createTextNode(std::string data) {
    return {
        .node_type = data,
        .children = {},
    };
}

Node createElementNode(std::string name, AttrMap attrs,
                       std::vector<Node> children) {
    ElementData data = {.tag_name = name, .attributes = attrs};
    return {
        .node_type = data,
        .children = children,
    };
}

Node createDocumentTypeNode(std::string name, std::string publicId = "",
                            std::string systemId = "") {
    DocumentType doctype_data = {.name = name};
    return {.node_type = doctype_data, .children = {}};
}

void printIndent(size_t depth, std::string indent = "  ") {
    std::cout << '\n';
    for (size_t i = 0; i < depth; i++)
        std::cout << indent;
}

void printTree(const Node &root, size_t depth = 0) {
    if (depth > 0)
        printIndent(depth);

    if (std::holds_alternative<DocumentType>(root.node_type)) {
        DocumentType doctype = std::get<DocumentType>(root.node_type);
        std::cout << "<!DOCTYPE " << doctype.name << ">\n";
        return;
    }

    if (std::holds_alternative<std::string>(root.node_type)) {
        std::cout << "\"" << std::get<std::string>(root.node_type) << "\"";
        return;
    }

    ElementData elem_data = std::get<ElementData>(root.node_type);
    std::cout << '<' << elem_data.tag_name;
    for (const auto &[key, value] : elem_data.attributes)
        std::cout << ' ' << key << "=\"" << value << '"';
    std::cout << '>';

    for (const Node &child : root.children)
        printTree(child, depth + 1);

    if (root.children.size())
        printIndent(depth);

    std::cout << "</" << elem_data.tag_name << '>';
}

int main() {
    printTree(createDocumentTypeNode("html"));

    Node root{createElementNode(
        "HTML", {{"LANG", "EN"}},
        {createElementNode(
            "DIV", {},
            {
                createElementNode("P", {{"ID", "subtitle"}, {"CLASS", "pink"}},
                                  {}),
                createElementNode(
                    "P", {},
                    {createTextNode("hello"),
                     createElementNode("A", {{"HREF", "https://example.com"}},
                                       {createTextNode("click me")}),
                     createElementNode(
                         "SPAN", {},
                         {createTextNode("something's in the water")})}),
                createElementNode("P", {}, {}),
            })})};
    printTree(root);
}
