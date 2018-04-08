#pragma once

#include "binarySearchTree.h"

//--------- Red-Black Tree ----------------------------------------------------

enum RBColor {
    rbcBlack,
    rbcRed
};

template <typename T>
struct RBData {
    RBData() {
        color = rbcBlack;
    }

    RBData(const T& data) {
        color = rbcBlack;
        value = data;
    }

    bool operator ==(const RBData<T>& rhs) const {
        return value == rhs.value;
    }

    bool operator <(const RBData<T>& rhs) const {
        return value < rhs.value;
    }

    RBColor color;
    T       value;
};

template <typename T>
struct RBTree : public BST<RBData<T>> {
    RBTree() {
    }

    RBTree(const RBTree<T>& tree) : BST<RBData<T>>(tree) {
    }

    RBTree<T>& operator =(const RBTree<T>& tree) {
        BST::operator =(tree);
        return *this;
    }

    pair<Node*,bool> insert(const T& item) {
        RBData<T> rbitem(item);
        pair<Node*, bool> ins = BST::insert(rbitem);
        if (!ins.second)
            return ins;

        Node* x = ins.first;
        Node* y;

        x->value.color = rbcRed;
        while ((x != root) && (x->parent->value.color == rbcRed)) {
            if (x->parent == x->parent->parent->left) {
                y = x->parent->parent->right;

                if (y->value.color == rbcRed) {
                    x->parent->value.color = rbcBlack;
                    y->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->right) {
                        x = x->parent;
                        rotateLeft(x);
                    }

                    x->parent->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    rotateRight(x->parent->parent);
                }
            } else {
                y = x->parent->parent->left;

                if (y->value.color == rbcRed) {
                    x->parent->value.color = rbcBlack;
                    y->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    x = x->parent->parent;
                } else {
                    if (x == x->parent->left) {
                        x = x->parent;
                        rotateRight(x);
                    }

                    x->parent->value.color = rbcBlack;
                    x->parent->parent->value.color = rbcRed;
                    rotateLeft(x->parent->parent);
                }
            }
        }
        root->value.color = rbcBlack;

        return ins;
    }

    bool erase(Node* node) {
        // "y" is a deleting node
        // "x" is child with which to replace y
        Node* z = node;
        if (z == sentinel)
            return false;

        Node *y, *x;

        if ((z->left == sentinel) || (z->right == sentinel))
            y = z;
        else
            y = next(z);

        // find child with which to replace y
        if (y->left != sentinel)
            x = y->left;
        else
            x = y->right;

        // splice child onto parent
        if (x != sentinel)
            x->parent = y->parent;

        if (y->parent == sentinel)
            // if deleting node is root, then replace root with x
            root = x;
        else {
            // splice in child node
            if (y == y->parent->left)
                y->parent->left = x;
            else
                y->parent->right = x;
        }

        // if needed, save y data
        if (y != z)
            swap(z->value.value, y->value.value);

        updateNodeToRoot(y->parent);

        // adjust tree under red-black rules
        if (y != sentinel && y->value.color == rbcBlack)
            deleteFixup(x);

        destroyNode(y);

        return true;
    }

    bool erase(const T& item) {
        return erase(find(item));
    }

protected:
    void rotateLeft(Node* node) {
        if (node == sentinel || node->right == sentinel)
            return;

        Node* y = node->right;

        // turn y's left subtree into node's right subtree
        node->right = y->left;
        if (y->left != sentinel)
            y->left->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == sentinel)
            root = y;
        else {
            if (node == node->parent->left)
                node->parent->left = y;
            else
                node->parent->right = y;
        }

        // put node on y's left
        y->left = node;
        node->parent = y;

        onUpdateNode(node);
        onUpdateNode(y);
    }

    void rotateRight(Node* node) {
        if (node == sentinel || node->left == sentinel)
            return;

        Node* y = node->left;

        // turn y's right subtree into node's left subtree
        node->left = y->right;
        if (y->right != sentinel)
            y->right->parent = node;

        // link node's parent to y
        y->parent = node->parent;
        if (node->parent == sentinel)
            root = y;
        else {
            if (node == node->parent->right)
                node->parent->right = y;
            else
                node->parent->left = y;
        }

        // put node on y's right
        y->right = node;
        node->parent = y;

        onUpdateNode(node);
        onUpdateNode(y);
    }

    void deleteFixup(Node* node) {
        if (node == sentinel)
            return;

        Node *w, *x = node;
        while ((x != root) && (x->value.color == rbcBlack)) {
            if (x == x->parent->left) {
                w = x->parent->right;

                if (w->value.color == rbcRed) {
                    w->value.color = rbcBlack;
                    x->parent->value.color = rbcRed;
                    rotateLeft(x->parent);
                    w = x->parent->right;
                }

                if ((w->left->value.color == rbcBlack) && (w->right->value.color == rbcBlack)) {
                    w->value.color = rbcRed;
                    x = x->parent;
                } else {
                    if (w->right->value.color == rbcBlack) {
                        w->left->value.color = rbcBlack;
                        w->value.color = rbcRed;
                        rotateRight(w);
                        w = x->parent->right;
                    }

                    w->value.color = x->parent->value.color;
                    x->parent->value.color = rbcBlack;
                    w->right->value.color = rbcBlack;
                    rotateLeft(x->parent);
                    x = root;
                }
            } else {
                w = x->parent->left;

                if (w->value.color == rbcRed) {
                    w->value.color = rbcBlack;
                    x->parent->value.color = rbcRed;
                    rotateRight(x->parent);
                    w = x->parent->left;
                }

                if ((w->right->value.color == rbcBlack) && (w->left->value.color == rbcBlack)) {
                    w->value.color = rbcRed;
                    x = x->parent;
                } else {
                    if (w->left->value.color == rbcBlack) {
                        w->right->value.color = rbcBlack;
                        w->value.color = rbcRed;
                        rotateLeft(w);
                        w = x->parent->left;
                    }

                    w->value.color = x->parent->value.color;
                    x->parent->value.color = rbcBlack;
                    w->left->value.color = rbcBlack;
                    rotateRight(x->parent);
                    x = root;
                }
            }
        }

        x->value.color = rbcBlack;
    }
};
