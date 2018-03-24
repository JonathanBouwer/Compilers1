#include <fstream>
#include <iostream>
#include <string>

#include "tree.h"
#include "lexer.cpp"
#include "parser.cpp"

int compile(char* fileName, bool silent, bool lexOnly=false) {
    try {
        auto s = tokenizeFile(fileName);
        if (lexOnly) {
            while (!s.empty()) {
                std::cout << s.top() << std::endl;
                s.pop();
            }
            return 0;
        }
        Tree<string> printTree("LIB");
        lib(s, printTree);
        printTree.printToFile("output.txt");
        if (!silent) 
            std::cout << "Tree successfully generated, check output.txt to see it" << endl;
        return 0;
    } catch(const string& s) {
        if (!silent) 
            std::cout << "Compilation error, check output.txt for more information" << endl;
        std::ofstream outputFile("output.txt", std::ofstream::binary);
        outputFile << s << std::endl;
        outputFile.close();
        return 1;
    }
}

int main(int argc, char** argv) {
    if (argc <= 1) { 
        std::cout << "No file chosen" << endl;
        return 1;
    } else if ((argc >= 3) && (argv[2][0] == '-')) {
        if (argv[2][1] == 'q') {
            return compile(argv[1], true);
        } else {
            return compile(argv[1], false, true);
        }
    } else {
        return compile(argv[1], false);
    }
}
