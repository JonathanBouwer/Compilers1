#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

/* Constant strings representing line shapes
** topInit = "├───"
** topPre  = "│   "
** botInit = "└───"
** botPre  = "    "
*/
const std::string topInitPrefix = "├───";
const std::string topPrefix = "│   ";
const std::string botInitPrefix = "└───";
const std::string botPrefix = "    ";

template <typename T>
class Tree {
    private:
        std::vector<Tree<T>> children;
        T value;
    public:
        Tree():value("") {}

        Tree(T val) : value(val) {}

        Tree(const Tree& other) : value(other.value),
                                  children(other.children) {}

        Tree(Tree&& other) : value(std::move(other.value)),
                             children(std::move(other.children)) {}

        Tree& operator=(const Tree& other) {
            value = other.value;
            children = other.children;
        }

        Tree& operator=(Tree&& other) {
            value = std::move(other.value);
            children = std::move(other.children);
        }

        ~Tree() {}

        void addChild(const Tree& child) {
            children.push_back(child);
        }
        
        void printToFile(const std::string& filename) {
            std::ofstream outputFile(filename, std::ofstream::binary);
            outputFile << *this;
            outputFile.close();
        }
        
        int numberOfChildren(){
            return children.size();
        }

        friend std::ostream& operator<<(std::ostream& out, const Tree<T>& tree) {
            out << prettyPrint(tree);
            return out;
        }
        
        friend std::string prettyPrint(const Tree<T>& tree, 
                                        std::string initialPrefix = "", 
                                        std::string prefix = "") {
            std::stringstream output;
            output << initialPrefix << tree.value  << "\n";
            if (tree.children.size() == 0) return output.str();
            
            int lastChildIndex = tree.children.size() - 1;
            for (int i = 0; i < lastChildIndex; i++) {
                Tree<T> child = tree.children[i];
                output << prettyPrint(child, prefix + topInitPrefix, prefix + topPrefix);
            }
            Tree<T> lastChild = tree.children[lastChildIndex];
            output << prettyPrint(lastChild, prefix + botInitPrefix, prefix + botPrefix);
            return output.str();
        }
};
