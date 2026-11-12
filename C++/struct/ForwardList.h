#pragma once

#include <fstream>
#include <iostream>

using namespace std;

template <typename T>
class ForwardList {
private:
    struct Node { T value; Node* next; };
    Node* head;

public:
    ForwardList() {
        head = nullptr;
    }

    ForwardList(const ForwardList&) = delete;
    ForwardList& operator=(const ForwardList&) = delete;

    bool FPUSH(const T& v) {
        head = new Node{ v, head };
        return true;
    }

    bool FPUSHB(const T& v) {
        Node* n = new Node{ v, nullptr };
        if (!head) {
            head = n;
            return true;
        }
        Node* p = head;
        while (p->next)
            p = p->next;
        p->next = n;
        return true;
    }

    bool FDEL() {
        if (!head)
            return false;
        Node* p = head;
        head = head->next;
        delete p;
        return true;
    }

    bool FDELVAL(const T& v) {
        Node* p = head;
        Node* prev = nullptr;
        while (p && !(p->value == v)) {
            prev = p;
            p = p->next;
        }
        if (!p)
            return false;
        if (prev)
            prev->next = p->next;
        else
            head = p->next;
        delete p;
        return true;
    }

    bool FFIND(const T& v) const {
        for (Node* p = head; p; p = p->next)
            if (p->value == v)
                return true;
        return false;
    }

    int FLen() const {
        int n = 0;
        for (Node* p = head; p; p = p->next)
            n++;
        return n;
    }

    void FPRINT() const {
        cout << "F[";
        for (Node* p = head; p; p = p->next)
            cout << p->value << " ";
        cout << "]" << endl;
    }

    bool saveText(const char* file) const {
        ofstream out(file);
        if (!out)
            return false;
        out << FLen() << '\n';
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
            FPUSHB(value);
        }
        return true;
    }

    bool saveBin(const char* file) const {
        ofstream out(file, ios::binary);
        if (!out)
            return false;
        int n = FLen();
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
            FPUSHB(value);
        }
        return true;
    }

    void clear() {
        while (head) {
            Node* p = head;
            head = head->next;
            delete p;
        }
    }

    ~ForwardList() { clear(); }
};
