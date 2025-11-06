#include "html_parser.h"
#include <iostream>
#include <stack>

Node* parseHTML(const std::string& html) {
    Node* root = new Node{ "root", "", {} };
    std::stack<Node*> stack;
    stack.push(root);

    std::string tag, text;
    bool inTag = false, closingTag = false;

    for (size_t i = 0; i < html.size(); ++i) {
        char c = html[i];

        if (c == '<') {
            if (!text.empty()) {
                Node* textNode = new Node{ "text", text, {} };
                stack.top()->children.push_back(textNode);
                text.clear();
            }
            inTag = true;
            closingTag = (i + 1 < html.size() && html[i + 1] == '/');
            tag.clear();
        }
        else if (c == '>' && inTag) {
            inTag = false;
            if (!closingTag) {
                Node* newNode = new Node{ tag, "", {} };
                stack.top()->children.push_back(newNode);
                stack.push(newNode);
            }
            else {
                if (!stack.empty()) stack.pop();
            }
        }
        else if (inTag) {
            if (c != '/') tag += c;
        }
        else {
            text += c;
        }
    }

    return root;
}

void printDOM(Node* node, int depth = 0) {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    std::cout << "<" << node->tag << ">";
    if (!node->text.empty()) std::cout << " " << node->text;
    std::cout << "\n";

    for (auto* child : node->children) {
        printDOM(child, depth + 1);
    }
}
