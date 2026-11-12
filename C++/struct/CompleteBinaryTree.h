#pragma once

#include <fstream>
#include <iostream>

using namespace std;

template <typename T>
class CompleteBinaryTree {
private:
    struct Node { T value; Node* left; Node* right; };
    Node* root;
    int size;

    Node* getNode(int index) const {
        if (index < 1 || !root)
            return nullptr;
        Node* p = root;
        int mask = 1;
        while (mask <= index / 2)
            mask *= 2;
        mask /= 2;
        while (mask > 0 && p) {
            p = (index & mask) ? p->right : p->left;
            mask /= 2;
        }
        return p;
    }

    void printInOrder(Node* p) const {
        if (!p)
            return;
        printInOrder(p->left);
        cout << p->value << " ";
        printInOrder(p->right);
    }

    void clearNode(Node* p) {
        if (!p)
            return;
        clearNode(p->left);
        clearNode(p->right);
        delete p;
    }

public:
    CompleteBinaryTree() {
        root = nullptr;
        size = 0;
    }

    CompleteBinaryTree(const CompleteBinaryTree&) = delete;
    CompleteBinaryTree& operator=(const CompleteBinaryTree&) = delete;

    bool TInsert(const T& value) {
        Node* n = new Node{ value, nullptr, nullptr };
        if (size == 0)
            root = n;
        else {
            Node* parent = getNode((size + 1) / 2);
            if (!parent) {
                delete n;
                return false;
            }
            if ((size + 1) % 2 == 0)
                parent->left = n;
            else
                parent->right = n;
        }
        size++;
        return true;
    }

    bool TFind(const T& value) const {
        for (int i = 1; i <= size; i++) {
            Node* p = getNode(i);
            if (p && p->value == value)
                return true;
        }
        return false;
    }

    bool TIsComplete() const { return true; }

    bool TIsFull() const {
        int n = size + 1;
        while (n > 1 && n % 2 == 0)
            n /= 2;
        return n == 1;
    }

    int TSize() const { return size; }

    void TPrint() const {
        printInOrder(root);
        cout << endl;
    }

    void TPrintTree() const {
        for (int i = 1; i <= size; i++) {
            Node* p = getNode(i);
            if (p)
                cout << p->value << " ";
        }
        cout << endl;
    }

    bool saveText(const char* file) const {
        ofstream out(file);
        if (!out)
            return false;
        out << size << '\n';
        for (int i = 1; i <= size; i++) {
            Node* p = getNode(i);
            if (p)
                out << p->value << '\n';
        }
        return !out.fail();
    }

    bool loadText(const char* file) {
        ifstream in(file);
        if (!in)
            return false;
        int n;
        if (!(in >> n) || n < 0)
            return false;
        clear();
        T value;
        for (int i = 0; i < n; i++) {
            if (!(in >> value)) {
                clear();
                return false;
            }
            TInsert(value);
        }
        return true;
    }

    bool saveBin(const char* file) const {
        ofstream out(file, ios::binary);
        if (!out)
            return false;
        out.write((char*)&size, sizeof(size));
        for (int i = 1; i <= size; i++) {
            Node* p = getNode(i);
            if (p)
                out.write((char*)&p->value, sizeof(T));
        }
        return !out.fail();
    }

    bool loadBin(const char* file) {
        ifstream in(file, ios::binary);
        if (!in)
            return false;
        int n;
        in.read((char*)&n, sizeof(n));
        if (!in || n < 0 || n > 1000000)
            return false;
        clear();
        for (int i = 0; i < n; i++) {
            T value;
            in.read((char*)&value, sizeof(T));
            if (!in) {
                clear();
                return false;
            }
            TInsert(value);
        }
        return true;
    }

    void clear() {
        clearNode(root);
        root = nullptr;
        size = 0;
    }

    ~CompleteBinaryTree() { clear(); }
};
