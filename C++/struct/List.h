
#pragma once
#include <iostream>
#include <fstream>

template <typename T>
class List {
public:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

private:
    Node* head;
    Node* tail;

public:
    List() : head(nullptr), tail(nullptr) {}

    ~List() {
        clear();
    }

    Node* LHead() const {
        return head;
    }

    Node* LTail() const {
        return tail;
    }

    bool contains(Node* node) const {
        Node* current = head;

        while (current) {
            if (current == node)
                return true;

            current = current->next;
        }

        return false;
    }

    void LPUSH(const T& value) {
        Node* node = new Node(value);

        node->next = head;

        if (head)
            head->prev = node;
        else
            tail = node;

        head = node;
    }

    void LPUSHB(const T& value) {
        Node* node = new Node(value);

        node->prev = tail;

        if (tail)
            tail->next = node;
        else
            head = node;

        tail = node;
    }

    bool LPUSH_AFTER(Node* node, const T& value) {
        if (!node || !contains(node))
            return false;

        if (node == tail) {
            LPUSHB(value);
            return true;
        }

        Node* newNode = new Node(value);

        newNode->next = node->next;
        newNode->prev = node;

        node->next->prev = newNode;
        node->next = newNode;

        return true;
    }

    bool LPUSH_BEFORE(Node* node, const T& value) {
        if (!node || !contains(node))
            return false;

        if (node == head) {
            LPUSH(value);
            return true;
        }

        Node* newNode = new Node(value);

        newNode->prev = node->prev;
        newNode->next = node;

        node->prev->next = newNode;
        node->prev = newNode;

        return true;
    }

    bool LDEL() {
        if (!head)
            return false;

        Node* temp = head;
        head = head->next;

        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;

        delete temp;
        return true;
    }

    bool LDELBACK() {
        if (!tail)
            return false;

        Node* temp = tail;
        tail = tail->prev;

        if (tail)
            tail->next = nullptr;
        else
            head = nullptr;

        delete temp;
        return true;
    }

    bool LDELNODE(Node* node) {
        if (!node || !contains(node))
            return false;

        if (node == head)
            return LDEL();

        if (node == tail)
            return LDELBACK();

        node->prev->next = node->next;
        node->next->prev = node->prev;

        delete node;
        return true;
    }

    bool LDEL_AFTER(Node* node) {
        if (!node || !contains(node) || !node->next)
            return false;

        return LDELNODE(node->next);
    }

    bool LDEL_BEFORE(Node* node) {
        if (!node || !contains(node) || !node->prev)
            return false;

        return LDELNODE(node->prev);
    }

    bool LDELVAL(const T& value) {
        Node* current = head;

        while (current) {
            if (current->data == value)
                return LDELNODE(current);

            current = current->next;
        }

        return false;
    }

    Node* LFIND(const T& value) const {
        Node* current = head;

        while (current) {
            if (current->data == value)
                return current;

            current = current->next;
        }

        return nullptr;
    }

    int LLen() const {
        int count = 0;
        Node* current = head;

        while (current) {
            count++;
            current = current->next;
        }

        return count;
    }

    void LPRINT() const {
        Node* current = head;

        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }

        std::cout << std::endl;
    }

    void LPRINTR() const {
        Node* current = tail;

        while (current) {
            std::cout << current->data << " ";
            current = current->prev;
        }

        std::cout << std::endl;
    }

    void LPRINTREC(Node* node) const {
        if (!node)
            return;

        std::cout << node->data << " ";
        LPRINTREC(node->next);
    }

    void clear() {
        while (head)
            LDEL();
    }

    void SaveText(const char* filename) const {
        std::ofstream file(filename);

        Node* current = head;
        while (current) {
            file << current->data << '\n';
            current = current->next;
        }
    }

    void LoadText(const char* filename) {
        std::ifstream file(filename);
        if (!file)
            return;

        clear();

        T value;
        while (file >> value)
            LPUSHB(value);
    }

    void SaveBin(const char* filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file)
            return;

        int size = LLen();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size));

        Node* current = head;
        while (current) {
            file.write(reinterpret_cast<const char*>(&current->data), sizeof(T));
            current = current->next;
        }
    }

    void LoadBin(const char* filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file)
            return;

        clear();

        int size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size));

        if (!file || size < 0)
            return;

        for (int i = 0; i < size; i++) {
            T value;
            file.read(reinterpret_cast<char*>(&value), sizeof(T));

            if (!file) {
                clear();
                return;
            }

            LPUSHB(value);
        }
    }
};
