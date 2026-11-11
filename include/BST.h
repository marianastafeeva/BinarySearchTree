#pragma once

#include "Node.h"
#include "Traversal.h"

#include <cstddef>
#include <vector>
#include <string>

namespace lab {

template <typename T>
class BST {
private:
    Node<T>* root_;
    std::size_t size_;

    Node<T>* insertImpl(Node<T>* node, const T& value, bool& inserted);
    Node<T>* searchImpl(Node<T>* node, const T& value) const;
    Node<T>* findMin(Node<T>* node) const;
    Node<T>* removeImpl(Node<T>* node, const T& value, bool& removed);
    void destroyTree(Node<T>* node);

    template <typename Func>
    void mapImpl(Node<T>* node, BST<T>& result, Func f) const;

    template <typename Predicate>
    void whereImpl(Node<T>* node, BST<T>& result, Predicate p) const;

    template <typename Func>
    T reduceImpl(Node<T>* node, Func f, const T& init) const;

    int getHeight(Node<T>* node) const;
    int getBalance(Node<T>* node) const;
    void updateHeight(Node<T>* node);
    Node<T>* rotateRight(Node<T>* y);
    Node<T>* rotateLeft(Node<T>* x);
    Node<T>* balance(Node<T>* node);

    void buildThreads(Node<T>* node, Node<T>*& prev);

    void saveImpl(Node<T>* node, const std::string& format, std::string& out) const;
    Node<T>* loadImpl(const std::string& data, size_t& pos);

public:
    BST();
    ~BST();

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;

    BST(BST&& other) noexcept;
    BST& operator=(BST&& other) noexcept;

    void Insert(const T& value);
    bool Search(const T& value) const;
    void Remove(const T& value);

    bool IsEmpty() const;
    std::size_t Size() const;
    int Height() const;

    std::vector<T> Traverse(lab::Order order) const;

    template <typename Func>
    BST<T> Map(Func f) const;

    template <typename Predicate>
    BST<T> Where(Predicate p) const;

    template <typename Func>
    T Reduce(Func f, const T& init) const;

    void BuildThreads();
    std::vector<T> ThreadedInOrder() const;

    std::string SaveToString(const std::string& format = "K(L)[R]") const;
    void LoadFromString(const std::string& data);
};

template <typename T>
BST<T>::BST() : root_(nullptr), size_(0) {}

template <typename T>
BST<T>::~BST() {
    destroyTree(root_);
}

template <typename T>
BST<T>::BST(BST&& other) noexcept : root_(other.root_), size_(other.size_) {
    other.root_ = nullptr;
    other.size_ = 0;
}

template <typename T>
BST<T>& BST<T>::operator=(BST&& other) noexcept {
    if (this != &other) {
        destroyTree(root_);
        root_ = other.root_;
        size_ = other.size_;
        other.root_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
void BST<T>::Insert(const T& value) {
    bool inserted = false;
    root_ = insertImpl(root_, value, inserted);
    if (inserted) {
        ++size_;
    }
}

template <typename T>
bool BST<T>::Search(const T& value) const {
    return searchImpl(root_, value) != nullptr;
}

template <typename T>
void BST<T>::Remove(const T& value) {
    bool removed = false;
    root_ = removeImpl(root_, value, removed);
    if (removed) {
        --size_;
    }
}

template <typename T>
bool BST<T>::IsEmpty() const {
    return root_ == nullptr;
}

template <typename T>
std::size_t BST<T>::Size() const {
    return size_;
}

template <typename T>
int BST<T>::Height() const {
    return getHeight(root_);
}

template <typename T>
std::vector<T> BST<T>::Traverse(lab::Order order) const {
    std::vector<T> result;
    lab::Traverse(root_, order, [&result](const T& val) { result.push_back(val); });
    return result;
}

template <typename T>
void BST<T>::BuildThreads() {
    Node<T>* prev = nullptr;
    buildThreads(root_, prev);
}

template <typename T>
std::vector<T> BST<T>::ThreadedInOrder() const {
    std::vector<T> result;
    if (root_ == nullptr) return result;

    Node<T>* curr = root_;
    
    while (curr->left != nullptr) {
        curr = curr->left;
    }

    while (curr != nullptr) {
        result.push_back(curr->value);
        curr = curr->next;
    }
    return result;
}

template <typename T>
Node<T>* BST<T>::insertImpl(Node<T>* node, const T& value, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new Node<T>(value);
    }
    if (value < node->value) {
        node->left = insertImpl(node->left, value, inserted);
    } else if (value > node->value) {
        node->right = insertImpl(node->right, value, inserted);
    }
    updateHeight(node);
    return balance(node);
}

template <typename T>
Node<T>* BST<T>::searchImpl(Node<T>* node, const T& value) const {
    if (node == nullptr) {
        return nullptr;
    }
    if (value == node->value) {
        return node;
    }
    if (value < node->value) {
        return searchImpl(node->left, value);
    }
    return searchImpl(node->right, value);
}

template <typename T>
Node<T>* BST<T>::findMin(Node<T>* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

template <typename T>
Node<T>* BST<T>::removeImpl(Node<T>* node, const T& value, bool& removed) {
    if (node == nullptr) {
        return nullptr;
    }
    if (value < node->value) {
        node->left = removeImpl(node->left, value, removed);
    } else if (value > node->value) {
        node->right = removeImpl(node->right, value, removed);
    } else {
        removed = true;
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            return nullptr;
        }
        if (node->left == nullptr) {
            Node<T>* temp = node->right;
            delete node;
            return temp;
        }
        if (node->right == nullptr) {
            Node<T>* temp = node->left;
            delete node;
            return temp;
        }
        Node<T>* minRight = findMin(node->right);
        node->value = minRight->value;
        node->right = removeImpl(node->right, minRight->value, removed);
        removed = true;
    }
    if (node != nullptr) {
        updateHeight(node);
        return balance(node);
    }
    return node;
}

template <typename T>
void BST<T>::destroyTree(Node<T>* node) {
    if (node != nullptr) {
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }
}

template <typename T>
int BST<T>::getHeight(Node<T>* node) const {
    return node ? node->height : 0;
}

template <typename T>
int BST<T>::getBalance(Node<T>* node) const {
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

template <typename T>
void BST<T>::updateHeight(Node<T>* node) {
    if (node) {
        int hl = getHeight(node->left);
        int hr = getHeight(node->right);
        node->height = 1 + (hl > hr ? hl : hr);
    }
}

template <typename T>
Node<T>* BST<T>::rotateRight(Node<T>* y) {
    Node<T>* x = y->left;
    Node<T>* T2 = x->right;

    x->right = y;
    y->left = T2;

    updateHeight(y);
    updateHeight(x);

    return x;
}

template <typename T>
Node<T>* BST<T>::rotateLeft(Node<T>* x) {
    Node<T>* y = x->right;
    Node<T>* T2 = y->left;

    y->left = x;
    x->right = T2;

    updateHeight(x);
    updateHeight(y);

    return y;
}

template <typename T>
Node<T>* BST<T>::balance(Node<T>* node) {
    if (!node) return nullptr;

    updateHeight(node);
    int bf = getBalance(node);

    if (bf > 1 && getBalance(node->left) >= 0) return rotateRight(node);
    if (bf < -1 && getBalance(node->right) <= 0) return rotateLeft(node);
    if (bf > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (bf < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}

template <typename T>
void BST<T>::buildThreads(Node<T>* node, Node<T>*& prev) {
    if (node == nullptr) return;

    buildThreads(node->left, prev);

    if (prev != nullptr) {
        prev->next = node;
    }
    prev = node;

    buildThreads(node->right, prev);
}

}

#include "BSTAlgorithms.h"
#include "BSTSerialization.h"