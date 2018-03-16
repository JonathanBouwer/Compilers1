#include <iostream>
#include <string>
#include "tree.h"

#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

void setupWindowsWideChar() {
    #ifdef _WIN32
    _setmode(_fileno(stdout), _O_WTEXT);
    #endif
}

void printPrettyTestTree() {
	Tree<std::string> tree1("Element 1");
	Tree<std::string> tree2("Element 2");
	Tree<std::string> tree3("Element 3");
	Tree<std::string> tree4("Element 4");
	Tree<std::string> tree5("Element 5");
	Tree<std::string> tree6("Element 6");
	Tree<std::string> tree7("Element 7");
	Tree<std::string> tree8("Element 8");

	tree1.addChild(tree2);
	tree1.addChild(tree7);
	tree2.addChild(tree3);
	tree2.addChild(tree4);
	tree2.addChild(tree6);
	tree4.addChild(tree5);
	tree7.addChild(tree8);

	std::cout << tree1;
}

int main(int argc, char** argv) {
	setupWindowsWideChar();

	//call to function to test pretty tree printing
	printPrettyTestTree();

    return 0;
}
