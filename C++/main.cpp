#include "struct/Array.h"
#include "struct/CompleteBinaryTree.h"
#include "struct/ForwardList.h"
#include "struct/HashTable.h"
#include "struct/List.h"
#include "struct/Queue.h"
#include "struct/Stack.h"

int main() {
  Array<int> a;
  a.MPush(10);
  a.MPush(20);
  a.MPushIdx(1, 15);
  a.MPrint();

  ForwardList<int> f;
  f.FPUSH(2);
  f.FPUSH(1);
  f.FPUSHB(3);
  f.FPRINT();

  List<int> l;
  l.LPUSHB(1);
  l.LPUSHB(2);
  l.LPUSH(0);
  l.LPRINT();
  l.LPRINTR();

  Stack<int> s;
  s.SPush(10);
  s.SPush(20);
  s.SPrint();

  Queue<int> q;
  q.QPush(10);
  q.QPush(20);
  q.QPrint();

  HashTable h;
  h.HInsert("name", "value");
  h.HPrint();

  CompleteBinaryTree<int> t;
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  t.TPrintTree();
  t.TPrint();

  a.saveText("/tmp/lab3_demo.txt");
  Array<int> a2;
  if (a2.loadText("/tmp/lab3_demo.txt")) {
    cout << "Text reload: ";
    a2.MPrint();
  }
  a.saveBin("/tmp/lab3_demo.bin");
  Array<int> a3;
  if (a3.loadBin("/tmp/lab3_demo.bin")) {
    cout << "Bin reload: ";
    a3.MPrint();
  }

  return 0;
}
