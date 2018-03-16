#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

/* Constant strings representing line shapes, uses wstring to support Box Drawing Characters
** topInit = "├───"
** topPre  = "│   "
** botInit = "└───"
** botPre  = "    "
*/
const std::wstring topInitPrefix = L"\u251C\u2500\u2500\u2500";
const std::wstring topPrefix = L"\u2502    ";
const std::wstring botInitPrefix = L"\u2514\u2500\u2500\u2500";
const std::wstring botPrefix = L"    ";

template <typename T>
std::wstring toWString(T s) {
    std::stringstream ss;
    ss << s;
    std::string treeValue = ss.str();
    return std::wstring(treeValue.begin(), treeValue.end());
}

template <typename T>
class Tree {
    private:
        std::vector<Tree<T>*> children;
        Tree<T>* parent;
        T value;
    public:
        Tree():value(""), parent(NULL) {}

        Tree(T val) : value(val), parent(NULL) {}

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

        void addChild(Tree& child) {
            children.push_back(&child);
        }

        friend std::wostream& operator<<(std::wostream& out, const Tree<T>& tree) {
            out << prettyPrint(tree);
            return out;
        }

        friend std::wstring prettyPrint(const Tree<T>& tree,
                                        std::wstring initialPrefix = L"",
                                        std::wstring prefix = L"") {
            std::wstringstream output;
            output << initialPrefix << toWString<T>(tree.value)  << L"\n";
            if (tree.children.size() == 0) return output.str();

            int lastChildIndex = tree.children.size() - 1;
            for (int i = 0; i < lastChildIndex; i++) {
                Tree<T> *child = tree.children[i];
                output << prettyPrint(*child, prefix + topInitPrefix, prefix + topPrefix);
            }
            Tree<T> *lastChild = tree.children[lastChildIndex];
            output << prettyPrint(*lastChild, prefix + botInitPrefix, prefix + botPrefix);
            return output.str();
        }
};
