#include "ui.h"

#include <iostream>
#include <limits>
#include <string>

namespace {

void printMenu() {
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "  Binary Search Tree - Manual Mode\n";
    std::cout << "========================================\n";
    std::cout << "  1. Insert value\n";
    std::cout << "  2. Search value\n";
    std::cout << "  3. Remove value\n";
    std::cout << "  4. Show tree size and status\n";
    std::cout << "  5. Print PreOrder (KLP)\n";
    std::cout << "  6. Print InOrder (LKP)\n";
    std::cout << "  7. Print PostOrder (LPK)\n";
    std::cout << "  8. Print ReverseInOrder (PKL)\n";
    std::cout << "  9. Map (multiply all by 2)\n";
    std::cout << " 10. Where (keep only even numbers)\n";
    std::cout << " 11. Reduce (sum all elements)\n";
    std::cout << " 12. Print tree height\n";
    std::cout << " 13. Build threads & print ThreadedInOrder\n";
    std::cout << " 14. Save to string & reload\n";
    std::cout << "  0. Exit\n";
    std::cout << "----------------------------------------\n";
    std::cout << "  Choice: ";
}

int readInt(const std::string& prompt) {
    int value = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            return value;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Invalid input. Please enter a number.\n";
    }
}

void printVector(const std::vector<int>& vec) {
    if (vec.empty()) {
        std::cout << "[] (empty)\n";
        return;
    }
    std::cout << "[ ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i];
        if (i != vec.size() - 1) std::cout << ", ";
    }
    std::cout << " ]\n";
}

} // namespace

namespace ui {

void runMainLoop(lab::BST<int>& tree) {
    while (true) {
        printMenu();
        
        int choice = -1;
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
        case 1: {
            int val = readInt("Enter value to insert: ");
            tree.Insert(val);
            std::cout << "Inserted " << val << ". Size is now " << tree.Size() << "\n";
            break;
        }
        case 2: {
            int val = readInt("Enter value to search: ");
            if (tree.Search(val)) {
                std::cout << "Value " << val << " FOUND in tree.\n";
            } else {
                std::cout << "Value " << val << " NOT FOUND.\n";
            }
            break;
        }
        case 3: {
            int val = readInt("Enter value to remove: ");
            tree.Remove(val);
            std::cout << "Attempted to remove " << val << ". Size is now " << tree.Size() << "\n";
            break;
        }
        case 4:
            std::cout << "Size: " << tree.Size() << "\n";
            std::cout << "Is Empty? " << (tree.IsEmpty() ? "Yes" : "No") << "\n";
            break;
        case 5:
            std::cout << "PreOrder (KLP): ";
            printVector(tree.Traverse(lab::Order::KLP));
            break;
        case 6:
            std::cout << "InOrder (LKP): ";
            printVector(tree.Traverse(lab::Order::LKP));
            break;
        case 7:
            std::cout << "PostOrder (LPK): ";
            printVector(tree.Traverse(lab::Order::LPK));
            break;
        case 8:
            std::cout << "ReverseInOrder (PKL): ";
            printVector(tree.Traverse(lab::Order::PKL));
            break;
        case 9: {
            std::cout << "Multiplying all elements by 2...\n";
            tree = tree.Map([](int x) { return x * 2; });
            std::cout << "Done. New size: " << tree.Size() << "\n";
            break;
        }
        case 10: {
            std::cout << "Filtering: keeping only even numbers...\n";
            tree = tree.Where([](int x) { return x % 2 == 0; });
            std::cout << "Done. New size: " << tree.Size() << "\n";
            break;
        }
        case 11: {
            int sum = tree.Reduce([](int acc, int val) { return acc + val; }, 0);
            std::cout << "Sum of all elements in tree: " << sum << "\n";
            break;
        }
        case 12:
            std::cout << "Tree height: " << tree.Height() << "\n";
            break;
        case 13:
            tree.BuildThreads();
            std::cout << "Threaded InOrder: ";
            printVector(tree.ThreadedInOrder());
            break;
        case 14: {
            std::string saved = tree.SaveToString("K(L)[R]");
            std::cout << "Saved string: " << saved << "\n";
            tree.LoadFromString(saved);
            std::cout << "Loaded back. Size: " << tree.Size() << "\n";
            break;
        }
        case 0:
            std::cout << "Exiting.\n";
            return;
        default:
            std::cout << "Unknown option: " << choice << "\n";
            break;
        }
    }
}

}