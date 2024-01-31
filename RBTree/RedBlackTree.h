

#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#include <iostream>
#include <iomanip>

enum Color { RED, BLACK };

template <typename T>
struct Node {
    T data;
    Color color;
    Node* parent;
    Node* left;
    Node* right;

    Node(const T& val)
        : data(val), color(RED), parent(nullptr), left(nullptr), right(nullptr) {}
};

template <typename T>
class RedBlackTree {
private:
    Node<T>* root;

    void deleteFixup(Node<T>* x);

    // Helper functions
    void leftRotate(Node<T>* x);
    void rightRotate(Node<T>* x);
    void insertFixup(Node<T>* z);
    void transplant(Node<T>* u, Node<T>* v);
    Node<T>* minimum(Node<T>* x);

public:
    RedBlackTree();
    ~RedBlackTree();

    void insert(const T& value);
    void remove(const T& value);
    void printTree(Node<T>* root, int indent = 0) const;
    Node<T>* getRoot() const;
    void destroyTree(Node<T>* node);
};

#endif // RED_BLACK_TREE_H
