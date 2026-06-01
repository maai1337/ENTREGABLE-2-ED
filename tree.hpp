#ifndef TREE_HPP
#define TREE_HPP

#include <vector>
#include <string>
#include <iostream>

class Tree {
public:
    struct Node {
        std::string data;
        Node* parent;
        std::vector<Node*> children;

        Node(const std::string& value, Node* p = nullptr);
    };

private:

    Node* rootNode;
    int treeSize;
    int k;

    void preOrder(Node* node, std::vector<std::string>& result);
    void postOrder(Node* node, std::vector<std::string>& result);
    void deleteSubtree(Node* node); //si se borra un nodo, se borran todos sus hijos y luego ese nodo.

public:
    Tree(int k);

    bool isEmpty();
    int size();

    std::string root();
    Node* rootNodePtr();
    Node* insertRoot(const std::string& value);
    Node* insertChild(Node* parent, const std::string& value);

    std::string parent(const std::string& value);
    std::vector<std::string> children(const std::string& value);

    bool insert(const std::string& parentValue, const std::string& value);
    bool remove(const std::string& value);
    
    Node* search(Node* node, const std::string& value);

    std::vector<std::string> preOrder();
    std::vector<std::string> postOrder();
    std::vector<std::string> inOrder();
};

#endif // TREE_HPP