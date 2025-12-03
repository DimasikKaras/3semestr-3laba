#include "avlTree.h"
#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;

AVLTree::AVLTreeNode::AVLTreeNode(const int value)
    : data(value), left(nullptr), right(nullptr), height(1) {}


AVLTree::AVLTree() : root(nullptr) {}

AVLTree::~AVLTree() {
    clear(root);
}

void AVLTree::insert(const int value) {
    root = insertRecursive(root, value);
}

void AVLTree::remove(const int value) {
    root = removeRecursive(root, value);
}

bool AVLTree::contains(const int value) const {
    const AVLTreeNode* current = root;
    while (current) {
        if (value == current->data) return true;
        if (value < current->data) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    return false;
}

int AVLTree::height(const AVLTreeNode* node) {
    return node ? node->height : 0;
}

int AVLTree::balanceFactor(const AVLTreeNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

void AVLTree::updateHeight(AVLTreeNode* node) {
    if (node) {
        node->height = 1 + max(height(node->left), height(node->right));
    }
}

AVLTree::AVLTreeNode* AVLTree::rotateRight(AVLTreeNode* y) {
    if (!y || !y->left) return y;

    AVLTreeNode* x = y->left;
    y->left = x->right;
    x->right = y;
    updateHeight(y);
    updateHeight(x);
    return x;
}

AVLTree::AVLTreeNode* AVLTree::rotateLeft(AVLTreeNode* x) {
    if (!x || !x->right) return x;

    AVLTreeNode* y = x->right;
    x->right = y->left;
    y->left = x;
    updateHeight(x);
    updateHeight(y);
    return y;
}

AVLTree::AVLTreeNode* AVLTree::balance(AVLTreeNode* node) {
    if (!node) return node;

    updateHeight(node);
    const int bf = balanceFactor(node);

    if (bf > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (bf < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

AVLTree::AVLTreeNode* AVLTree::findMin(AVLTreeNode* node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

void AVLTree::clear(const AVLTreeNode* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

AVLTree::AVLTreeNode* AVLTree::insertRecursive(AVLTreeNode* node, const int value) {
    if (!node) return new AVLTreeNode(value);
    
    if (value < node->data) {
        node->left = insertRecursive(node->left, value);
    } else if (value > node->data) {
        node->right = insertRecursive(node->right, value);
    }
    
    return balance(node);
}

AVLTree::AVLTreeNode* AVLTree::removeRecursive(AVLTreeNode* node, const int value) {
    if (!node) return node;
    
    if (value < node->data) {
        node->left = removeRecursive(node->left, value);
    } else if (value > node->data) {
        node->right = removeRecursive(node->right, value);
    } else {
        if (!node->left || !node->right) {
            AVLTreeNode* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        } else {
            const AVLTreeNode* temp = findMin(node->right);
            node->data = temp->data;
            node->right = removeRecursive(node->right, temp->data);
        }
    }
    
    return balance(node);
}

void AVLTree::print() const {
    cout << "AVL Tree:" << endl;
    printTree(root);
    cout << endl;
}

void AVLTree::printTree(const AVLTreeNode* node, const string& prefix, const bool isLeft) {
    if (node) {
        cout << prefix;
        cout << (isLeft ? "├──" : "└──" );

        cout << node->data << "(h:" << node->height << ")" << endl;

        printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
        printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}


void AVLTree::getElementsRecursive(const AVLTreeNode* node, std::vector<int>& elements) {
    if (node) {
        getElementsRecursive(node->left, elements);
        elements.push_back(node->data);
        getElementsRecursive(node->right, elements);
    }
}

std::vector<int> AVLTree::getElements() const {
    std::vector<int> elements;
    getElementsRecursive(root, elements);
    return elements;
}