#include <fstream>
#include <iostream>
#include <string>

#include "tree.h"
#include "lexer.cpp"
#include "parser.cpp"

void printPrettyTestTree() {
    Tree<std::string> tree1("Element 1");
    Tree<std::string> tree2("Element 2");
    Tree<std::string> tree3("Element 3");
    Tree<std::string> tree4("Element 4");
    Tree<std::string> tree5("Element 5");
    Tree<std::string> tree6("Element 6");
    Tree<std::string> tree7("Element 7");
    Tree<std::string> tree8("Element 8");

    tree4.addChild(tree5);
    tree2.addChild(tree3);
    tree2.addChild(tree4);
    tree2.addChild(tree6);
    tree7.addChild(tree8);
    tree1.addChild(tree7);
    tree1.addChild(tree2);

    tree1.printToFile("output.txt");
}

void printPrettyTestTokenStream() {
    std::ofstream outputFile("output.txt", std::ofstream::binary | std::ofstream::app);
    try {
        auto s = tokenizeFile("sample/tesProg.ys");
        // while (!s.empty()) {
        //     outputFile << s.top() << std::endl;
        //     s.pop();
        // }
        if (s.top().type != TokenType::KEYWORD_PRETEXT){
            string error = "Expected the Pretext Keyword, I did. This is why you fail \n";
    		throw string(error);
        }
        Tree<string> printTree(s.top().literal);
        s.pop();
        Parser p;
        std::cout << "About to go itno tree " << std::endl;
        p.lib(s, printTree);
        printTree.printToFile("output.txt");
    } catch(const string& s) {
        std::ofstream outputFile("output.txt", std::ofstream::binary);
        outputFile << s << std::endl;
    }

    outputFile.close();
}

int main(int argc, char** argv) {
//    printPrettyTestTree();
    printPrettyTestTokenStream();
    return 0;
}
