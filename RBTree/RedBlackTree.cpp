#include "RedBlackTree.h"

// Constructor: Initializes an empty Red-Black Tree
template <typename T>
RedBlackTree<T>::RedBlackTree() : root(nullptr) {}

// Destructor: Deallocates memory used by the Red-Black Tree
template <typename T>
RedBlackTree<T>::~RedBlackTree() {
    destroyTree(root);
}

// Transplants a subtree rooted at 'targetNode' into the position of the subtree rooted at 'sourceNode'
template <typename T>
void RedBlackTree<T>::transplant(Node<T>* sourceNode, Node<T>* targetNode) {
    // Update the root if 'sourceNode' is the root
    if (sourceNode->parent == nullptr)
        root = targetNode;
    else if (sourceNode == sourceNode->parent->left)
        sourceNode->parent->left = targetNode;
    else
        sourceNode->parent->right = targetNode;

    // Update the parent pointer of 'targetNode'
    if (targetNode != nullptr)
        targetNode->parent = sourceNode->parent;
}

// Finds the node with the minimum key in a given subtree
template <typename T>
Node<T>* RedBlackTree<T>::minimum(Node<T>* startingNode) {
    while (startingNode->left != nullptr)
        startingNode = startingNode->left;
    return startingNode;
}

// Removes a node with a given value from the Red-Black Tree
template <typename T>
void RedBlackTree<T>::remove(const T& value) {
    Node<T>* currentNode = root;
    while (currentNode != nullptr) {
        if (value < currentNode->data)
            currentNode = currentNode->left;
        else if (value > currentNode->data)
            currentNode = currentNode->right;
        else {
            Node<T>* nodeToDelete = currentNode;
            Color originalColor = nodeToDelete->color;
            Node<T>* successorNode;

            if (currentNode->left == nullptr) {
                successorNode = currentNode->right;
                transplant(currentNode, currentNode->right);
            }
            else if (currentNode->right == nullptr) {
                successorNode = currentNode->left;
                transplant(currentNode, currentNode->left);
            }
            else {
                successorNode = minimum(currentNode->right);
                originalColor = successorNode->color;
                Node<T>* successorRightSubtree = successorNode->right;

                if (successorNode->parent == currentNode)
                    successorRightSubtree->parent = successorNode;
                else {
                    transplant(successorNode, successorNode->right);
                    successorNode->right = currentNode->right;
                    successorNode->right->parent = successorNode;
                }
                transplant(currentNode, successorNode);
                successorNode->left = currentNode->left;
                successorNode->left->parent = successorNode;
                successorNode->color = currentNode->color;
            }

            if (originalColor == BLACK)
                deleteFixup(successorNode);

            delete nodeToDelete;
            return;
        }
    }
}

// Rotates a node to the left in the Red-Black Tree
template <typename T>
void RedBlackTree<T>::leftRotate(Node<T>* rotatingNode) {
    Node<T>* rightChild = rotatingNode->right;
    rotatingNode->right = rightChild->left;

    if (rightChild->left != nullptr)
        rightChild->left->parent = rotatingNode;

    rightChild->parent = rotatingNode->parent;

    if (rotatingNode->parent == nullptr)
        root = rightChild;
    else if (rotatingNode == rotatingNode->parent->left)
        rotatingNode->parent->left = rightChild;
    else
        rotatingNode->parent->right = rightChild;

    rightChild->left = rotatingNode;
    rotatingNode->parent = rightChild;
}

// Rotates a node to the right in the Red-Black Tree
template <typename T>
void RedBlackTree<T>::rightRotate(Node<T>* rotatingNode) {
    Node<T>* leftChild = rotatingNode->left;
    rotatingNode->left = leftChild->right;

    if (leftChild->right != nullptr)
        leftChild->right->parent = rotatingNode;

    leftChild->parent = rotatingNode->parent;

    if (rotatingNode->parent == nullptr)
        root = leftChild;
    else if (rotatingNode == rotatingNode->parent->left)
        rotatingNode->parent->left = leftChild;
    else
        rotatingNode->parent->right = leftChild;

    leftChild->right = rotatingNode;
    rotatingNode->parent = leftChild;
}

// Fixes violations of the Red-Black Tree properties after a node deletion
template <typename T>
void RedBlackTree<T>::deleteFixup(Node<T>* nodeToFix) {
    while (nodeToFix != root && (nodeToFix == nullptr || nodeToFix->color == BLACK)) {
        if (nodeToFix == nodeToFix->parent->left) {
            Node<T>* siblingNode = nodeToFix->parent->right;
            if (siblingNode != nullptr && siblingNode->color == RED) {
                siblingNode->color = BLACK;
                nodeToFix->parent->color = RED;
                leftRotate(nodeToFix->parent);
                siblingNode = nodeToFix->parent->right;
            }
            if ((siblingNode->left == nullptr || siblingNode->left->color == BLACK) &&
                (siblingNode->right == nullptr || siblingNode->right->color == BLACK)) {
                siblingNode->color = RED;
                nodeToFix = nodeToFix->parent;
            }
            else {
                if (siblingNode->right == nullptr || siblingNode->right->color == BLACK) {
                    if (siblingNode->left != nullptr)
                        siblingNode->left->color = BLACK;
                    siblingNode->color = RED;
                    rightRotate(siblingNode);
                    siblingNode = nodeToFix->parent->right;
                }
                siblingNode->color = nodeToFix->parent->color;
                nodeToFix->parent->color = BLACK;
                if (siblingNode->right != nullptr)
                    siblingNode->right->color = BLACK;
                leftRotate(nodeToFix->parent);
                nodeToFix = root;
            }
        }
        else {
            // Symmetric case
            Node<T>* siblingNode = nodeToFix->parent->left;
            if (siblingNode != nullptr && siblingNode->color == RED) {
                siblingNode->color = BLACK;
                nodeToFix->parent->color = RED;
                rightRotate(nodeToFix->parent);
                siblingNode = nodeToFix->parent->left;
            }
            if ((siblingNode->right == nullptr || siblingNode->right->color == BLACK) &&
                (siblingNode->left == nullptr || siblingNode->left->color == BLACK)) {
                siblingNode->color = RED;
                nodeToFix = nodeToFix->parent;
            }
            else {
                if (siblingNode->left == nullptr || siblingNode->left->color == BLACK) {
                    if (siblingNode->right != nullptr)
                        siblingNode->right->color = BLACK;
                    siblingNode->color = RED;
                    leftRotate(siblingNode);
                    siblingNode = nodeToFix->parent->left;
                }
                siblingNode->color = nodeToFix->parent->color;
                nodeToFix->parent->color = BLACK;
                if (siblingNode->left != nullptr)
                    siblingNode->left->color = BLACK;
                rightRotate(nodeToFix->parent);
                nodeToFix = root;
            }
        }
    }
}

// Fixes violations of the Red-Black Tree properties after a node insertion
template <typename T>
void RedBlackTree<T>::insertFixup(Node<T>* newNode) {
    while (newNode->parent != nullptr && newNode->parent->color == RED) {
        if (newNode->parent == newNode->parent->parent->left) {
            Node<T>* uncleNode = newNode->parent->parent->right;
            if (uncleNode != nullptr && uncleNode->color == RED) {
                newNode->parent->color = BLACK;
                uncleNode->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            }
            else {
                if (newNode == newNode->parent->right) {
                    newNode = newNode->parent;
                    leftRotate(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                rightRotate(newNode->parent->parent);
            }
        }
        else {
            // Symmetric case
            Node<T>* uncleNode = newNode->parent->parent->left;
            if (uncleNode != nullptr && uncleNode->color == RED) {
                newNode->parent->color = BLACK;
                uncleNode->color = BLACK;
                newNode->parent->parent->color = RED;
                newNode = newNode->parent->parent;
            }
            else {
                if (newNode == newNode->parent->left) {
                    newNode = newNode->parent;
                    rightRotate(newNode);
                }
                newNode->parent->color = BLACK;
                newNode->parent->parent->color = RED;
                leftRotate(newNode->parent->parent);
            }
        }
    }

    root->color = BLACK;
}

// Inserts a new node with the given value into the Red-Black Tree
template <typename T>
void RedBlackTree<T>::insert(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    Node<T>* parent = nullptr;
    Node<T>* current = root;

    // Find the correct position for the new node
    while (current != nullptr) {
        parent = current;
        if (newNode->data < current->data)
            current = current->left;
        else
            current = current->right;
    }

    newNode->parent = parent;

    // Update the parent's child pointer
    if (parent == nullptr)
        root = newNode;
    else if (newNode->data < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    // Fix any Red-Black Tree violations
    insertFixup(newNode);
}

// Prints the Red-Black Tree in a readable format
template <typename T>
void RedBlackTree<T>::printTree(Node<T>* rootNode, int indent) const {
    if (rootNode != nullptr) {
        if (rootNode->right != nullptr)
            printTree(rootNode->right, indent + 4);
        if (indent > 0)
            std::cout << std::setw(indent) << ' ';
        std::cout << rootNode->data << " (" << (rootNode->color == RED ? "RED" : "BLACK") << ")\n";
        if (rootNode->left != nullptr)
            printTree(rootNode->left, indent + 4);
    }
}

// Returns the root of the Red-Black Tree
template <typename T>
Node<T>* RedBlackTree<T>::getRoot() const {
    return root;
}

// Recursively deallocates memory used by the Red-Black Tree
template <typename T>
void RedBlackTree<T>::destroyTree(Node<T>* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

// Explicit instantiation to avoid linker errors
template class RedBlackTree<int>;
