#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <string>
#include <vector>

class AVLTree {
    struct AVLTreeNode {
        int data;
        AVLTreeNode* left;
        AVLTreeNode* right;
        int height;
        explicit AVLTreeNode(int value);
    };

    AVLTreeNode* root;

    static AVLTreeNode* insertRecursive(AVLTreeNode* node, int value);

    static AVLTreeNode* removeRecursive(AVLTreeNode* node, int value);

    static void getElementsRecursive(const AVLTreeNode* node, std::vector<int>& elements);

public:
    AVLTree();
    ~AVLTree();

    void insert(int value);
    void remove(int value);
    bool contains(int value) const;
    std::vector<int> getElements() const;

    static int height(const AVLTreeNode* node);

    static int balanceFactor(const AVLTreeNode* node);

    static void updateHeight(AVLTreeNode* node);

    static AVLTreeNode* rotateRight(AVLTreeNode* y);

    static AVLTreeNode* rotateLeft(AVLTreeNode* x);

    static AVLTreeNode* balance(AVLTreeNode* node);

    static AVLTreeNode* findMin(AVLTreeNode* node);

    static void clear(const AVLTreeNode* node);

    void print() const;

    static void printTree(const AVLTreeNode* node, const std::string& prefix = "", bool isLeft = false);
    AVLTreeNode* getRoot() const { return root; }
};

#endif