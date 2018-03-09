#include <iostream>
#include <string>
#include "tree.h"

int main(int argc, char** argv) {
    Tree<std::string> tree("Hello World!\n");
    std::cout << tree;
    return 0;
}