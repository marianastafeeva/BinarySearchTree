#pragma once

#include <utility>

namespace lab {

template <typename T>
struct Node {
    T value;
    Node* left;
    Node* right;
    Node* next;
    int height;

    explicit Node(const T& val)
        : value(val), left(nullptr), right(nullptr), next(nullptr), height(1) {}

    explicit Node(T&& val)
        : value(std::move(val)), left(nullptr), right(nullptr), next(nullptr), height(1) {}
};

}