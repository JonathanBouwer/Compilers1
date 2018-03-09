#pragma once
#include <vector>
#include <iostream>

template <typename T>
class Tree {
    private:
        std::vector<Tree<T>> children;
        T value;
    public:
        Tree() {}
        
        Tree(T val) : value(val) {}
        
        Tree(const Tree& other) : value(other.value), children(other.children) {}
        
        Tree(Tree&& other) : value(std::move(other.value)), children(std::move(other.children)) {}
        
        Tree& operator=(const Tree& other) {
            value = other.value;
            children = other.children;
        }
        
        Tree& operator=(Tree&& other) {
            value = std::move(other.value);
            children = std::move(other.children);
        }
        
        ~Tree() {}
        
        friend std::ostream& operator<<(std::ostream& out, const Tree<T>& tree) {
            out << tree.value;
            return out;
        }
};