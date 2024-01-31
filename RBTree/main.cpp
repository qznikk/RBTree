
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "RedBlackTree.cpp"

int main() {
    {
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

        RedBlackTree<int> rbTree;

        // Insert some values into the tree
        rbTree.insert(10);
        rbTree.insert(20);
        rbTree.insert(30);
        rbTree.insert(15);
        rbTree.insert(25);
        rbTree.printTree(rbTree.getRoot());
        rbTree.remove(20);

        // Print the Red-Black Tree
        std::cout << "Red-Black Tree:\n";
        rbTree.printTree(rbTree.getRoot());
    }

    _CrtDumpMemoryLeaks();

    return 0;
}
