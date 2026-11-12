#pragma once

#include <fstream>
#include <iostream>

using namespace std;

template <typename T>
class Stack {
private:
    struct Node { T value; Node* next; };
    Node* head;

public:
    Stack() {
        head = nullptr;
    }

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;

    bool SEmpty() const { return head == nullptr; }

    bool SPush(const T& v) {
        head = new Node{ v, head };
        return true;
    }

    bool SPop() {
        if (SEmpty())
            return false;
        Node* p = head;
        head = head->next;
        delete p;
        return true;
    }

    bool STop(T& out) const {
        if (SEmpty())
            return false;
        out = head->value;
        return true;
    }

    int SLen() const {
        int n = 0;
        for (Node* p = head; p; p = p->next)
            n++;
        return n;
    }

    void SPrint() const {
        cout << "S[";
        for (Node* p = head; p; p = p->next)
            cout << p->value << " ";
        cout << "]" << endl;
    }

    bool saveText(const char* file) const {
        ofstream out(file);
        if (!out)
            return false;
        out << SLen() << '\n';
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
        T* tmp = new T[n > 0 ? n : 1];
        for (int i = 0; i < n; i++) {
            if (!(in >> tmp[i])) {
                delete[] tmp;
                clear();
                return false;
            }
        }
        for (int i = n - 1; i >= 0; i--)
            SPush(tmp[i]);
        delete[] tmp;
        return true;
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
        T* tmp = new T[n > 0 ? n : 1];
        for (int i = 0; i < n; i++) {
            if (!in.read((char*)&tmp[i], sizeof(T))) {
                delete[] tmp;
                clear();
                return false;
            }
        }
        for (int i = n - 1; i >= 0; i--)
            SPush(tmp[i]);
        delete[] tmp;
        return true;
    }
    
    bool saveBin(const char* file) const {
        ofstream out(file, ios::binary);
        if (!out)
            return false;
        int n = SLen();
        out.write((char*)&n, sizeof(n));
        for (Node* p = head; p; p = p->next)
            out.write((char*)&p->value, sizeof(T));
        return !out.fail();
    }

    void clear() {
        while (head) {
            Node* p = head;
            head = head->next;
            delete p;
        }
    }

    ~Stack() { clear(); }
};
