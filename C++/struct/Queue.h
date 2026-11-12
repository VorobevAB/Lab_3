#pragma once

#include <fstream>
#include <iostream>

using namespace std;

template <typename T>
class Queue {
private:
    struct Node { T value; Node* next; };
    Node* head;
    Node* tail;

public:
    Queue() {
        head = nullptr;
        tail = nullptr;
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    bool QEmpty() const { return head == nullptr; }

    bool QPush(const T& v) {
        Node* n = new Node{ v, nullptr };
        if (tail)
            tail->next = n;
        else
            head = n;
        tail = n;
        return true;
    }

    bool QPop() {
        if (QEmpty())
            return false;
        Node* p = head;
        head = head->next;
        if (!head)
            tail = nullptr;
        delete p;
        return true;
    }

    bool QFront(T& out) const {
        if (QEmpty())
            return false;
        out = head->value;
        return true;
    }

    int QLen() const {
        int n = 0;
        for (Node* p = head; p; p = p->next)
            n++;
        return n;
    }

    void QPrint() const {
        cout << "Q[";
        for (Node* p = head; p; p = p->next)
            cout << p->value << " ";
        cout << "]" << endl;
    }

    bool saveText(const char* file) const {
        ofstream out(file);
        if (!out)
            return false;
        out << QLen() << '\n';
        for (Node* p = head; p; p = p->next)
            out << p->value << '\n';
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
            QPush(value);
        }
        return true;
    }

    bool saveBin(const char* file) const {
        ofstream out(file, ios::binary);
        if (!out)
            return false;
        int n = QLen();
        out.write((char*)&n, sizeof(n));
        for (Node* p = head; p; p = p->next)
            out.write((char*)&p->value, sizeof(T));
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
            QPush(value);
        }
        return true;
    }

    void clear() {
        while (head) {
            Node* p = head;
            head = head->next;
            delete p;
        }
        tail = nullptr;
    }

    ~Queue() { clear(); }
};
