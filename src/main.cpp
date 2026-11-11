#include "BST.h"
#include "ui.h"

int main() {
    lab::BST<int> tree;
    ui::runMainLoop(tree);
    return 0;
}