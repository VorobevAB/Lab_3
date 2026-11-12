#pragma once

#include <fstream>
#include <iostream>

using namespace std;

template <typename T> class List {
private:
  struct Node {
    T value;
    Node *next;
    Node *prev;
  };
  Node *head;
  Node *tail;

public:
  List() {
    head = nullptr;
    tail = nullptr;
  }

  List(const List &) = delete;
  List &operator=(const List &) = delete;

  bool LPUSH(const T &v) {
    Node *n = new Node{v, head, nullptr};
    if (head)
      head->prev = n;
    else
      tail = n;
    head = n;
    return true;
  }

  bool LPUSHB(const T &v) {
    Node *n = new Node{v, nullptr, tail};
    if (tail)
      tail->next = n;
    else
      head = n;
    tail = n;
    return true;
  }

  bool LDEL() {
    if (!head)
      return false;
    Node *p = head;
    head = head->next;
    if (head)
      head->prev = nullptr;
    else
      tail = nullptr;
    delete p;
    return true;
  }

  bool LDELBACK() {
    if (!tail)
      return false;
    Node *p = tail;
    tail = tail->prev;
    if (tail)
      tail->next = nullptr;
    else
      head = nullptr;
    delete p;
    return true;
  }

  bool LDELVAL(const T &v) {
    Node *p = head;
    while (p && !(p->value == v))
      p = p->next;
    if (!p)
      return false;
    if (p->prev)
      p->prev->next = p->next;
    else
      head = p->next;
    if (p->next)
      p->next->prev = p->prev;
    else
      tail = p->prev;
    delete p;
    return true;
  }

  bool LFIND(const T &v) const {
    for (Node *p = head; p; p = p->next)
      if (p->value == v)
        return true;
    return false;
  }

  int LLen() const {
    int n = 0;
    for (Node *p = head; p; p = p->next)
      n++;
    return n;
  }

  void LPRINT() const {
    cout << "L[";
    for (Node *p = head; p; p = p->next)
      cout << p->value << " ";
    cout << "]" << endl;
  }

  void LPRINTR() const {
    cout << "L[";
    for (Node *p = tail; p; p = p->prev)
      cout << p->value << " ";
    cout << "]" << endl;
  }

  bool saveText(const char *file) const {
    ofstream out(file);
    if (!out)
      return false;
    out << LLen() << '\n';
    for (Node *p = head; p; p = p->next)
      out << p->value << '\n';
    return !out.fail();
  }

  bool loadText(const char *file) {
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
      LPUSHB(value);
    }
    return true;
  }

  bool saveBin(const char *file) const {
    ofstream out(file, ios::binary);
    if (!out)
      return false;
    int n = LLen();
    out.write((char *)&n, sizeof(n));
    for (Node *p = head; p; p = p->next)
      out.write((char *)&p->value, sizeof(T));
    return !out.fail();
  }

  bool loadBin(const char *file) {
    ifstream in(file, ios::binary);
    if (!in)
      return false;
    int n;
    in.read((char *)&n, sizeof(n));
    if (!in || n < 0 || n > 1000000)
      return false;
    clear();
    for (int i = 0; i < n; i++) {
      T value;
      in.read((char *)&value, sizeof(T));
      if (!in) {
        clear();
        return false;
      }
      LPUSHB(value);
    }
    return true;
  }

  void clear() {
    while (head) {
      Node *p = head;
      head = head->next;
      delete p;
    }
    tail = nullptr;
  }

  ~List() { clear(); }
};
