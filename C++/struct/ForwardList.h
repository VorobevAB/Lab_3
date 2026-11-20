
#pragma once
#include <iostream>
#include <fstream>

template <typename T>
class ForwardList {
public:
    struct Node {
        T data;
        Node* next;

        Node(const T& value) : data(value), next(nullptr) {}
    };

private:
    Node* head;

public:
    ForwardList() : head(nullptr) {}

    ~ForwardList() {
        clear();
    }

    Node* FHead() const {
        return head;
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

    void FPUSH(const T& value) {
        Node* node = new Node(value);
        node->next = head;
        head = node;
    }

    void FPUSHB(const T& value) {
        Node* node = new Node(value);

        if (!head) {
            head = node;
            return;
        }

        Node* current = head;
        while (current->next)
            current = current->next;

        current->next = node;
    }

    bool FPUSH_AFTER(Node* node, const T& value) {
        if (!node || !contains(node))
            return false;

        Node* newNode = new Node(value);
        newNode->next = node->next;
        node->next = newNode;
        return true;
    }

    bool FPUSH_BEFORE(Node* node, const T& value) {
        if (!node || !contains(node))
            return false;

        if (node == head) {
            FPUSH(value);
            return true;
        }

        Node* current = head;
        while (current && current->next != node)
            current = current->next;

        if (!current)
            return false;

        Node* newNode = new Node(value);
        newNode->next = node;
        current->next = newNode;
        return true;
    }

    bool FDEL() {
        if (!head)
            return false;

        Node* temp = head;
        head = head->next;
        delete temp;
        return true;
    }

    bool FDELBACK() {
        if (!head)
            return false;

        if (!head->next)
            return FDEL();

        Node* current = head;
        while (current->next->next)
            current = current->next;

        delete current->next;
        current->next = nullptr;
        return true;
    }

    bool FDELNODE(Node* node) {
        if (!node || !contains(node))
            return false;

        if (node == head)
            return FDEL();

        Node* current = head;
        while (current && current->next != node)
            current = current->next;

        if (!current)
            return false;

        current->next = node->next;
        delete node;
        return true;
    }

    bool FDEL_AFTER(Node* node) {
        if (!node || !contains(node) || !node->next)
            return false;

        Node* temp = node->next;
        node->next = temp->next;
        delete temp;
        return true;
    }

    bool FDEL_BEFORE(Node* node) {
        if (!node || !contains(node) || node == head)
            return false;

        Node* prev = head;
        while (prev->next != node)
            prev = prev->next;

        if (prev == head)
            return FDEL();

        Node* beforePrev = head;
        while (beforePrev->next != prev)
            beforePrev = beforePrev->next;

        beforePrev->next = node;
        delete prev;
        return true;
    }

    bool FDELVAL(const T& value) {
        Node* current = head;
        Node* prev = nullptr;

        while (current) {
            if (current->data == value) {
                if (prev)
                    prev->next = current->next;
                else
                    head = current->next;

                delete current;
                return true;
            }

            prev = current;
            current = current->next;
        }

        return false;
    }

    Node* FFIND(const T& value) const {
        Node* current = head;

        while (current) {
            if (current->data == value)
                return current;

            current = current->next;
        }

        return nullptr;
    }

    int FLen() const {
        int count = 0;
        Node* current = head;

        while (current) {
            count++;
            current = current->next;
        }

        return count;
    }

    void FPRINT() const {
        Node* current = head;

        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }

        std::cout << std::endl;
    }

    void FPRINTREC(Node* node) const {
        if (!node)
            return;

        std::cout << node->data << " ";
        FPRINTREC(node->next);
    }

    void clear() {
        while (head)
            FDEL();
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
            FPUSHB(value);
    }

    void SaveBin(const char* filename) const {
        std::ofstream file(filename, std::ios::binary);
        if (!file)
            return;

        int size = FLen();
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

            FPUSHB(value);
        }
    }
};
