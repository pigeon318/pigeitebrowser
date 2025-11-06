#pragma once
#include <string>
#include <vector>

struct Node {
    std::string tag;
    std::string text;
    std::vector<Node*> children;
};
