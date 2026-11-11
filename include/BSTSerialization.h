#pragma once

#include "BST.h"
#include <string>
#include <cctype>

namespace lab {

template <typename T>
std::string BST<T>::SaveToString(const std::string& format) const {
    std::string result;
    saveImpl(root_, format, result);
    return result;
}

template <typename T>
void BST<T>::LoadFromString(const std::string& data) {
    destroyTree(root_);
    root_ = nullptr;
    size_ = 0;
    size_t pos = 0;
    root_ = loadImpl(data, pos);
}

template <typename T>
void BST<T>::saveImpl(Node<T>* node, const std::string& format, std::string& out) const {
    if (node == nullptr) return;

    for (size_t i = 0; i < format.size(); ++i) {
        char c = format[i];
        if (c == 'K') {
            out += std::to_string(node->value);
        } else if (c == 'L') {
            if (node->left != nullptr) {
                out += '(';
                saveImpl(node->left, format, out);
                out += ')';
            }
        } else if (c == 'R') {
            if (node->right != nullptr) {
                out += '[';
                saveImpl(node->right, format, out);
                out += ']';
            }
        }
    }
}

template <typename T>
Node<T>* BST<T>::loadImpl(const std::string& data, size_t& pos) {
    if (pos >= data.size()) return nullptr;

    size_t start = pos;
    while (pos < data.size() && (std::isdigit(data[pos]) || data[pos] == '-')) {
        pos++;
    }
    if (pos == start) return nullptr;

    T value = static_cast<T>(std::stod(data.substr(start, pos - start)));
    Node<T>* node = new Node<T>(value);
    ++size_;

    if (pos < data.size() && data[pos] == '(') {
        pos++;
        node->left = loadImpl(data, pos);
        if (pos < data.size() && data[pos] == ')') pos++;
    }

    if (pos < data.size() && data[pos] == '[') {
        pos++;
        node->right = loadImpl(data, pos);
        if (pos < data.size() && data[pos] == ']') pos++;
    }

    updateHeight(node);
    return balance(node);
}

}