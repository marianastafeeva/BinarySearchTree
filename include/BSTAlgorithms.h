#pragma once

#include "BST.h"
#include <vector>

namespace lab {

template <typename T>
template <typename Func>
BST<T> BST<T>::Map(Func f) const {
    BST<T> result;
    mapImpl(root_, result, f);
    return result;
}

template <typename T>
template <typename Predicate>
BST<T> BST<T>::Where(Predicate p) const {
    BST<T> result;
    whereImpl(root_, result, p);
    return result;
}

template <typename T>
template <typename Func>
T BST<T>::Reduce(Func f, const T& init) const {
    return reduceImpl(root_, f, init);
}

template <typename T>
template <typename Func>
void BST<T>::mapImpl(Node<T>* node, BST<T>& result, Func f) const {
    if (node != nullptr) {
        result.Insert(f(node->value));
        mapImpl(node->left, result, f);
        mapImpl(node->right, result, f);
    }
}

template <typename T>
template <typename Predicate>
void BST<T>::whereImpl(Node<T>* node, BST<T>& result, Predicate p) const {
    if (node != nullptr) {
        if (p(node->value)) {
            result.Insert(node->value);
        }
        whereImpl(node->left, result, p);
        whereImpl(node->right, result, p);
    }
}

template <typename T>
template <typename Func>
T BST<T>::reduceImpl(Node<T>* node, Func f, const T& init) const {
    if (node == nullptr) {
        return init;
    }
    T leftResult = reduceImpl(node->left, f, init);
    T currentResult = f(leftResult, node->value);
    return reduceImpl(node->right, f, currentResult);
}

}