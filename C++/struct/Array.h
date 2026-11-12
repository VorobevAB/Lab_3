#pragma once

#include <fstream>
#include <iostream>

using namespace std;

template <typename T> class Array {
private:
  T *data;
  int size;
  int capacity;

public:
  Array() {
    data = nullptr;
    size = 0;
    capacity = 0;
  }

  Array(const Array &) = delete;
  Array &operator=(const Array &) = delete;

  bool MPush(const T &n) {
    if (size == capacity) {
      int newCapacity = capacity == 0 ? 1 : capacity * 2;
      T *nd = new T[newCapacity];
      for (int i = 0; i < size; i++)
        nd[i] = data[i];
      delete[] data;
      data = nd;
      capacity = newCapacity;
    }
    data[size++] = n;
    return true;
  }

  bool MPushIdx(int idx, const T &n) {
    if (idx < 0 || idx > size)
      return false;
    MPush(T{});
    for (int i = size - 1; i > idx; i--)
      data[i] = data[i - 1];
    data[idx] = n;
    return true;
  }

  bool MGet(int idx, T &out) const {
    if (idx < 0 || idx >= size)
      return false;
    out = data[idx];
    return true;
  }

  bool MSetInd(int idx, const T &n) {
    if (idx < 0 || idx >= size)
      return false;
    data[idx] = n;
    return true;
  }

  bool MDelInd(int idx) {
    if (idx < 0 || idx >= size)
      return false;
    for (int i = idx; i < size - 1; i++)
      data[i] = data[i + 1];
    size--;
    return true;
  }

  int MLen() const { return size; }

  void MPrint() const {
    cout << "M[";
    for (int i = 0; i < size; i++)
      cout << data[i] << " ";
    cout << "]" << endl;
  }

  bool saveText(const char *file) const {
    ofstream out(file);
    if (!out)
      return false;
    out << size << '\n';
    for (int i = 0; i < size; i++)
      out << data[i] << '\n';
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
      MPush(value);
    }
    return true;
  }

  bool saveBin(const char *file) const {
    ofstream out(file, ios::binary);
    if (!out)
      return false;
    out.write((char *)&size, sizeof(size));
    out.write((char *)data, sizeof(T) * size);
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
      MPush(value);
    }
    return true;
  }

  void clear() {
    delete[] data;
    data = nullptr;
    size = 0;
    capacity = 0;
  }

  ~Array() { clear(); }
};
