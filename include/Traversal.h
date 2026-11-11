#pragma once

#include "Node.h"
#include <array>

namespace lab {

enum class Order {
    KLP, KPL, LPK, LKP, PLK, PKL
};

template <typename T, typename Func>
void Traverse(Node<T>* node, Order order, Func visit) {
    if (node == nullptr) return;

    std::array<int, 3> sequence;
    
    switch (order) {
        case Order::KLP: sequence = {0, 1, 2}; break;
        case Order::KPL: sequence = {0, 2, 1}; break;
        case Order::LPK: sequence = {1, 2, 0}; break;
        case Order::LKP: sequence = {1, 0, 2}; break;
        case Order::PLK: sequence = {2, 1, 0}; break;
        case Order::PKL: sequence = {2, 0, 1}; break;
    }

    for (int step : sequence) {
        if (step == 0) {
            visit(node->value);
        } else if (step == 1) {
            Traverse(node->left, order, visit);
        } else if (step == 2) {
            Traverse(node->right, order, visit);
        }
    }
}

}