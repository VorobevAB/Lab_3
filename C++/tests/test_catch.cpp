#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "struct/Array.h"
#include "struct/CompleteBinaryTree.h"
#include "struct/ForwardList.h"
#include "struct/HashTable.h"
#include "struct/List.h"
#include "struct/Queue.h"
#include "struct/Stack.h"

TEST_CASE("Array push get len", "[Array]") {
  Array<int> a;
  a.MPush(10);
  a.MPush(30);
  a.MPushIdx(1, 20);
  REQUIRE(a.MLen() == 3);
  int v;
  REQUIRE(a.MGet(1, v));
  REQUIRE(v == 20);
}

TEST_CASE("Array push idx errors", "[Array]") {
  Array<int> a;
  a.MPush(1);
  REQUIRE_FALSE(a.MPushIdx(-1, 0));
  REQUIRE_FALSE(a.MPushIdx(5, 0));
  REQUIRE(a.MPushIdx(0, 9));
  REQUIRE(a.MPushIdx(a.MLen(), 8));
}

TEST_CASE("Array get set del errors", "[Array]") {
  Array<int> a;
  int v;
  REQUIRE_FALSE(a.MGet(0, v));
  REQUIRE_FALSE(a.MSetInd(0, 1));
  REQUIRE_FALSE(a.MDelInd(0));
}

TEST_CASE("Array del clear", "[Array]") {
  Array<int> a;
  a.MPush(1);
  a.MPush(2);
  a.MPush(3);
  REQUIRE(a.MDelInd(0));
  int v;
  REQUIRE(a.MGet(0, v));
  REQUIRE(v == 2);
  a.clear();
  REQUIRE(a.MLen() == 0);
  REQUIRE(a.MPush(5));
  REQUIRE(a.MLen() == 1);
}

TEST_CASE("Array serialize text", "[Array]") {
  Array<int> a;
  a.MPush(1);
  a.MPush(2);
  a.MPush(3);
  REQUIRE(a.saveText("/tmp/lab3_c_a.txt"));
  Array<int> b;
  REQUIRE(b.loadText("/tmp/lab3_c_a.txt"));
  REQUIRE(b.MLen() == 3);
  int v;
  REQUIRE(b.MGet(2, v));
  REQUIRE(v == 3);
}

TEST_CASE("Array serialize bin", "[Array]") {
  Array<int> a;
  a.MPush(7);
  a.MPush(8);
  REQUIRE(a.saveBin("/tmp/lab3_c_a.bin"));
  Array<int> b;
  REQUIRE(b.loadBin("/tmp/lab3_c_a.bin"));
  REQUIRE(b.MLen() == 2);
  int v;
  REQUIRE(b.MGet(1, v));
  REQUIRE(v == 8);
}

TEST_CASE("Array load bad file", "[Array]") {
  Array<int> a;
  REQUIRE_FALSE(a.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(a.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST_CASE("ForwardList push find del", "[ForwardList]") {
  ForwardList<int> f;
  f.FPUSH(2);
  f.FPUSH(1);
  f.FPUSHB(3);
  REQUIRE(f.FLen() == 3);
  REQUIRE(f.FFIND(3));
  REQUIRE(f.FDELVAL(2));
  REQUIRE_FALSE(f.FFIND(2));
  REQUIRE_FALSE(f.FDELVAL(99));
}

TEST_CASE("ForwardList del empty", "[ForwardList]") {
  ForwardList<int> f;
  REQUIRE_FALSE(f.FDEL());
}

TEST_CASE("ForwardList del head tail middle", "[ForwardList]") {
  ForwardList<int> f;
  f.FPUSHB(1);
  f.FPUSHB(2);
  f.FPUSHB(3);
  REQUIRE(f.FDEL());
  REQUIRE_FALSE(f.FFIND(1));
  REQUIRE(f.FDELVAL(3));
  REQUIRE_FALSE(f.FFIND(3));
  REQUIRE(f.FDELVAL(2));
  REQUIRE(f.FLen() == 0);
}

TEST_CASE("ForwardList serialize text", "[ForwardList]") {
  ForwardList<int> f;
  f.FPUSHB(1);
  f.FPUSHB(2);
  REQUIRE(f.saveText("/tmp/lab3_c_f.txt"));
  ForwardList<int> g;
  REQUIRE(g.loadText("/tmp/lab3_c_f.txt"));
  REQUIRE(g.FLen() == 2);
}

TEST_CASE("ForwardList serialize bin", "[ForwardList]") {
  ForwardList<int> f;
  f.FPUSHB(7);
  f.FPUSHB(8);
  REQUIRE(f.saveBin("/tmp/lab3_c_f.bin"));
  ForwardList<int> g;
  REQUIRE(g.loadBin("/tmp/lab3_c_f.bin"));
  REQUIRE(g.FLen() == 2);
}

TEST_CASE("ForwardList load bad file", "[ForwardList]") {
  ForwardList<int> f;
  REQUIRE_FALSE(f.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(f.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST_CASE("List push del", "[List]") {
  List<int> l;
  l.LPUSHB(1);
  l.LPUSHB(2);
  l.LPUSH(0);
  REQUIRE(l.LLen() == 3);
  REQUIRE(l.LDEL());
  REQUIRE(l.LDELBACK());
}

TEST_CASE("List del val", "[List]") {
  List<int> l;
  l.LPUSHB(1);
  l.LPUSHB(2);
  l.LPUSHB(3);
  REQUIRE(l.LDELVAL(1));
  REQUIRE(l.LDELVAL(3));
  REQUIRE(l.LDELVAL(2));
  REQUIRE_FALSE(l.LDELVAL(99));
}

TEST_CASE("List del empty", "[List]") {
  List<int> l;
  REQUIRE_FALSE(l.LDEL());
  REQUIRE_FALSE(l.LDELBACK());
}

TEST_CASE("List serialize text", "[List]") {
  List<int> l;
  l.LPUSHB(10);
  l.LPUSHB(20);
  REQUIRE(l.saveText("/tmp/lab3_c_l.txt"));
  List<int> l2;
  REQUIRE(l2.loadText("/tmp/lab3_c_l.txt"));
  REQUIRE(l2.LLen() == 2);
}

TEST_CASE("List serialize bin", "[List]") {
  List<int> l;
  l.LPUSHB(5);
  l.LPUSHB(6);
  REQUIRE(l.saveBin("/tmp/lab3_c_l.bin"));
  List<int> l2;
  REQUIRE(l2.loadBin("/tmp/lab3_c_l.bin"));
  REQUIRE(l2.LLen() == 2);
}

TEST_CASE("List load bad file", "[List]") {
  List<int> l;
  REQUIRE_FALSE(l.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(l.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST_CASE("Stack push pop top", "[Stack]") {
  Stack<int> s;
  int v;
  REQUIRE(s.SEmpty());
  REQUIRE_FALSE(s.STop(v));
  REQUIRE_FALSE(s.SPop());
  s.SPush(1);
  s.SPush(2);
  REQUIRE(s.SLen() == 2);
  REQUIRE(s.STop(v));
  REQUIRE(v == 2);
  REQUIRE(s.SPop());
  REQUIRE(s.STop(v));
  REQUIRE(v == 1);
}

TEST_CASE("Stack pop to empty", "[Stack]") {
  Stack<int> s;
  s.SPush(1);
  REQUIRE(s.SPop());
  REQUIRE(s.SEmpty());
  REQUIRE_FALSE(s.SPop());
}

TEST_CASE("Stack serialize text", "[Stack]") {
  Stack<int> s;
  s.SPush(1);
  s.SPush(2);
  s.SPush(3);
  REQUIRE(s.saveText("/tmp/lab3_c_s.txt"));
  Stack<int> s2;
  REQUIRE(s2.loadText("/tmp/lab3_c_s.txt"));
  REQUIRE(s2.SLen() == 3);
  int v;
  REQUIRE(s2.STop(v));
  REQUIRE(v == 3);
}

TEST_CASE("Stack serialize bin", "[Stack]") {
  Stack<int> s;
  s.SPush(1);
  s.SPush(2);
  s.SPush(3);
  REQUIRE(s.saveBin("/tmp/lab3_c_s.bin"));
  Stack<int> s2;
  REQUIRE(s2.loadBin("/tmp/lab3_c_s.bin"));
  REQUIRE(s2.SLen() == 3);
  int v;
  REQUIRE(s2.STop(v));
  REQUIRE(v == 3);
}

TEST_CASE("Stack load bad file", "[Stack]") {
  Stack<int> s;
  REQUIRE_FALSE(s.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(s.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST_CASE("Queue push pop front", "[Queue]") {
  Queue<int> q;
  int v;
  REQUIRE(q.QEmpty());
  REQUIRE_FALSE(q.QFront(v));
  REQUIRE_FALSE(q.QPop());
  q.QPush(1);
  q.QPush(2);
  REQUIRE(q.QLen() == 2);
  REQUIRE(q.QFront(v));
  REQUIRE(v == 1);
  REQUIRE(q.QPop());
  REQUIRE(q.QFront(v));
  REQUIRE(v == 2);
}

TEST_CASE("Queue pop to empty", "[Queue]") {
  Queue<int> q;
  q.QPush(1);
  REQUIRE(q.QPop());
  REQUIRE(q.QEmpty());
  REQUIRE_FALSE(q.QPop());
}

TEST_CASE("Queue serialize text", "[Queue]") {
  Queue<int> q;
  q.QPush(1);
  q.QPush(2);
  q.QPush(3);
  REQUIRE(q.saveText("/tmp/lab3_c_q.txt"));
  Queue<int> q2;
  REQUIRE(q2.loadText("/tmp/lab3_c_q.txt"));
  REQUIRE(q2.QLen() == 3);
  int v;
  REQUIRE(q2.QFront(v));
  REQUIRE(v == 1);
}

TEST_CASE("Queue serialize bin", "[Queue]") {
  Queue<int> q;
  q.QPush(1);
  q.QPush(2);
  q.QPush(3);
  REQUIRE(q.saveBin("/tmp/lab3_c_q.bin"));
  Queue<int> q2;
  REQUIRE(q2.loadBin("/tmp/lab3_c_q.bin"));
  REQUIRE(q2.QLen() == 3);
  int v;
  REQUIRE(q2.QFront(v));
  REQUIRE(v == 1);
}

TEST_CASE("Queue load bad file", "[Queue]") {
  Queue<int> q;
  REQUIRE_FALSE(q.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(q.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST_CASE("HashTable insert find update", "[HashTable]") {
  HashTable h;
  string v;
  h.HInsert("one", "1");
  h.HInsert("two", "2");
  REQUIRE(h.HSize() == 2);
  REQUIRE(h.HFind("one", v));
  REQUIRE(v == "1");
  REQUIRE_FALSE(h.HFind("nope", v));
  h.HInsert("one", "11");
  REQUIRE(h.HFind("one", v));
  REQUIRE(v == "11");
}

TEST_CASE("HashTable delete", "[HashTable]") {
  HashTable h;
  h.HInsert("a", "A");
  h.HInsert("k", "K");
  REQUIRE(h.HDelete("a"));
  REQUIRE(h.HDelete("k"));
  REQUIRE_FALSE(h.HDelete("nope"));
}

TEST_CASE("HashTable collisions", "[HashTable]") {
  HashTable h;
  string v;
  h.HInsert("a", "A");
  h.HInsert("k", "K");
  REQUIRE(h.HFind("a", v));
  REQUIRE(v == "A");
  REQUIRE(h.HFind("k", v));
  REQUIRE(v == "K");
  REQUIRE(h.HSize() == 2);
}

TEST_CASE("HashTable serialize text", "[HashTable]") {
  HashTable h;
  h.HInsert("x", "1");
  h.HInsert("y", "2");
  REQUIRE(h.saveText("/tmp/lab3_c_h.txt"));
  HashTable h2;
  REQUIRE(h2.loadText("/tmp/lab3_c_h.txt"));
  REQUIRE(h2.HSize() == 2);
  string v;
  REQUIRE(h2.HFind("x", v));
  REQUIRE(v == "1");
}

TEST_CASE("HashTable serialize bin", "[HashTable]") {
  HashTable h;
  h.HInsert("x", "1");
  h.HInsert("yy", "22");
  REQUIRE(h.saveBin("/tmp/lab3_c_h.bin"));
  HashTable h2;
  REQUIRE(h2.loadBin("/tmp/lab3_c_h.bin"));
  REQUIRE(h2.HSize() == 2);
  string v;
  REQUIRE(h2.HFind("yy", v));
  REQUIRE(v == "22");
}

TEST_CASE("HashTable load bad file", "[HashTable]") {
  HashTable h;
  REQUIRE_FALSE(h.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(h.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST_CASE("Tree insert find", "[Tree]") {
  CompleteBinaryTree<int> t;
  REQUIRE_FALSE(t.TFind(1));
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  REQUIRE(t.TSize() == 7);
  for (int i = 1; i <= 7; i++)
    REQUIRE(t.TFind(i));
  REQUIRE_FALSE(t.TFind(99));
}

TEST_CASE("Tree is full", "[Tree]") {
  CompleteBinaryTree<int> t;
  REQUIRE(t.TIsFull());
  t.TInsert(1);
  REQUIRE(t.TIsFull());
  t.TInsert(2);
  REQUIRE_FALSE(t.TIsFull());
  t.TInsert(3);
  REQUIRE(t.TIsFull());
  for (int i = 4; i <= 7; i++)
    t.TInsert(i);
  REQUIRE(t.TIsFull());
  t.TInsert(8);
  REQUIRE_FALSE(t.TIsFull());
}

TEST_CASE("Tree clear reuse", "[Tree]") {
  CompleteBinaryTree<int> t;
  t.TInsert(1);
  t.TInsert(2);
  t.clear();
  REQUIRE(t.TSize() == 0);
  REQUIRE(t.TInsert(42));
  REQUIRE(t.TFind(42));
}

TEST_CASE("Tree serialize text", "[Tree]") {
  CompleteBinaryTree<int> t;
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  REQUIRE(t.saveText("/tmp/lab3_c_t.txt"));
  CompleteBinaryTree<int> t2;
  REQUIRE(t2.loadText("/tmp/lab3_c_t.txt"));
  REQUIRE(t2.TSize() == 7);
  REQUIRE(t2.TIsFull());
}

TEST_CASE("Tree serialize bin", "[Tree]") {
  CompleteBinaryTree<int> t;
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  REQUIRE(t.saveBin("/tmp/lab3_c_t.bin"));
  CompleteBinaryTree<int> t2;
  REQUIRE(t2.loadBin("/tmp/lab3_c_t.bin"));
  REQUIRE(t2.TSize() == 7);
  REQUIRE(t2.TFind(7));
}

TEST_CASE("Tree load bad file", "[Tree]") {
  CompleteBinaryTree<int> t;
  REQUIRE_FALSE(t.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  REQUIRE_FALSE(t.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}
