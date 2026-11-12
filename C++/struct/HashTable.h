#pragma once

#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class HashTable {
private:
    struct Node { string key, value; Node* next; };
    static const int COUNT = 10;
    Node* table[COUNT];

    int hash(const string& key) const {
        unsigned int h = 0;
        for (unsigned int i = 0; i < key.size(); i++)
            h = h * 31 + static_cast<unsigned char>(key[i]);
        return h % COUNT;
    }

public:
    HashTable() {
        for (int i = 0; i < COUNT; i++)
            table[i] = nullptr;
    }

    HashTable(const HashTable&) = delete;
    HashTable& operator=(const HashTable&) = delete;

    bool HInsert(const string& key, const string& value) {
        int i = hash(key);
        for (Node* p = table[i]; p; p = p->next) {
            if (p->key == key) {
                p->value = value;
                return true;
            }
        }
        table[i] = new Node{ key, value, table[i] };
        return true;
    }

    bool HFind(const string& key, string& value) const {
        for (Node* p = table[hash(key)]; p; p = p->next) {
            if (p->key == key) {
                value = p->value;
                return true;
            }
        }
        return false;
    }

    bool HDelete(const string& key) {
        int i = hash(key);
        Node* p = table[i];
        Node* prev = nullptr;
        while (p && p->key != key) {
            prev = p;
            p = p->next;
        }
        if (!p)
            return false;
        if (prev)
            prev->next = p->next;
        else
            table[i] = p->next;
        delete p;
        return true;
    }

    int HSize() const {
        int n = 0;
        for (int i = 0; i < COUNT; i++)
            for (Node* p = table[i]; p; p = p->next)
                n++;
        return n;
    }

    void HPrint() const {
        for (int i = 0; i < COUNT; i++) {
            cout << i << ": ";
            for (Node* p = table[i]; p; p = p->next)
                cout << p->key << "=" << p->value << " ";
            cout << endl;
        }
    }

    bool saveText(const char* file) const {
        ofstream out(file);
        if (!out)
            return false;
        out << HSize() << '\n';
        for (int i = 0; i < COUNT; i++)
            for (Node* p = table[i]; p; p = p->next)
                out << p->key << ' ' << p->value << '\n';
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
        for (int i = 0; i < n; i++) {
            string k, v;
            if (!(in >> k >> v)) {
                clear();
                return false;
            }
            HInsert(k, v);
        }
        return true;
    }

    bool saveBin(const char* file) const {
        ofstream out(file, ios::binary);
        if (!out)
            return false;
        int n = HSize();
        out.write((char*)&n, sizeof(n));
        for (int i = 0; i < COUNT; i++) {
            for (Node* p = table[i]; p; p = p->next) {
                int klen = (int)p->key.size();
                int vlen = (int)p->value.size();
                out.write((char*)&klen, sizeof(klen));
                out.write(p->key.data(), klen);
                out.write((char*)&vlen, sizeof(vlen));
                out.write(p->value.data(), vlen);
            }
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
            int klen;
            if (!in.read((char*)&klen, sizeof(klen)) || klen < 0 || klen > 1000000) {
                clear();
                return false;
            }
            string k(klen, '\0');
            if (klen > 0 && !in.read(&k[0], klen)) {
                clear();
                return false;
            }
            int vlen;
            if (!in.read((char*)&vlen, sizeof(vlen)) || vlen < 0 || vlen > 1000000) {
                clear();
                return false;
            }
            string v(vlen, '\0');
            if (vlen > 0 && !in.read(&v[0], vlen)) {
                clear();
                return false;
            }
            HInsert(k, v);
        }
        return true;
    }

    void clear() {
        for (int i = 0; i < COUNT; i++) {
            while (table[i]) {
                Node* p = table[i];
                table[i] = table[i]->next;
                delete p;
            }
        }
    }

    ~HashTable() { clear(); }
};
