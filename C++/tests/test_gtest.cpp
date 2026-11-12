#include "struct/Array.h"
#include "struct/CompleteBinaryTree.h"
#include "struct/ForwardList.h"
#include "struct/HashTable.h"
#include "struct/List.h"
#include "struct/Queue.h"
#include "struct/Stack.h"
#include <gtest/gtest.h>

TEST(Array, PushGetLen) {
  Array<int> a;
  a.MPush(10);
  a.MPush(30);
  a.MPushIdx(1, 20);
  EXPECT_EQ(a.MLen(), 3);
  int v;
  ASSERT_TRUE(a.MGet(1, v));
  EXPECT_EQ(v, 20);
}

TEST(Array, PushIdxErrors) {
  Array<int> a;
  a.MPush(1);
  EXPECT_FALSE(a.MPushIdx(-1, 0));
  EXPECT_FALSE(a.MPushIdx(5, 0));
  EXPECT_TRUE(a.MPushIdx(0, 9));
  EXPECT_TRUE(a.MPushIdx(a.MLen(), 8));
}

TEST(Array, GetSetDelErrors) {
  Array<int> a;
  int v;
  EXPECT_FALSE(a.MGet(0, v));
  EXPECT_FALSE(a.MSetInd(0, 1));
  EXPECT_FALSE(a.MDelInd(0));
}

TEST(Array, DelAndClear) {
  Array<int> a;
  a.MPush(1);
  a.MPush(2);
  a.MPush(3);
  EXPECT_TRUE(a.MDelInd(0));
  int v;
  ASSERT_TRUE(a.MGet(0, v));
  EXPECT_EQ(v, 2);
  a.clear();
  EXPECT_EQ(a.MLen(), 0);
  EXPECT_TRUE(a.MPush(5));
  EXPECT_EQ(a.MLen(), 1);
}

TEST(Array, SerializeText) {
  Array<int> a;
  a.MPush(1);
  a.MPush(2);
  a.MPush(3);
  ASSERT_TRUE(a.saveText("/tmp/lab3_a.txt"));
  Array<int> b;
  ASSERT_TRUE(b.loadText("/tmp/lab3_a.txt"));
  EXPECT_EQ(b.MLen(), 3);
  int v;
  ASSERT_TRUE(b.MGet(2, v));
  EXPECT_EQ(v, 3);
}

TEST(Array, SerializeBin) {
  Array<int> a;
  a.MPush(7);
  a.MPush(8);
  ASSERT_TRUE(a.saveBin("/tmp/lab3_a.bin"));
  Array<int> b;
  ASSERT_TRUE(b.loadBin("/tmp/lab3_a.bin"));
  EXPECT_EQ(b.MLen(), 2);
  int v;
  ASSERT_TRUE(b.MGet(1, v));
  EXPECT_EQ(v, 8);
}

TEST(Array, LoadBadFile) {
  Array<int> a;
  EXPECT_FALSE(a.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(a.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST(ForwardList, PushFindDel) {
  ForwardList<int> f;
  f.FPUSH(2);
  f.FPUSH(1);
  f.FPUSHB(3);
  EXPECT_EQ(f.FLen(), 3);
  EXPECT_TRUE(f.FFIND(3));
  EXPECT_TRUE(f.FDELVAL(2));
  EXPECT_FALSE(f.FFIND(2));
  EXPECT_FALSE(f.FDELVAL(99));
}

TEST(ForwardList, DelEmpty) {
  ForwardList<int> f;
  EXPECT_FALSE(f.FDEL());
}

TEST(ForwardList, DelHeadTailMiddle) {
  ForwardList<int> f;
  f.FPUSHB(1);
  f.FPUSHB(2);
  f.FPUSHB(3);
  EXPECT_TRUE(f.FDEL());
  EXPECT_FALSE(f.FFIND(1));
  EXPECT_TRUE(f.FDELVAL(3));
  EXPECT_FALSE(f.FFIND(3));
  EXPECT_TRUE(f.FDELVAL(2));
  EXPECT_EQ(f.FLen(), 0);
}

TEST(ForwardList, SerializeText) {
  ForwardList<int> f;
  f.FPUSHB(1);
  f.FPUSHB(2);
  ASSERT_TRUE(f.saveText("/tmp/lab3_f.txt"));
  ForwardList<int> g;
  ASSERT_TRUE(g.loadText("/tmp/lab3_f.txt"));
  EXPECT_EQ(g.FLen(), 2);
}

TEST(ForwardList, SerializeBin) {
  ForwardList<int> f;
  f.FPUSHB(7);
  f.FPUSHB(8);
  ASSERT_TRUE(f.saveBin("/tmp/lab3_f.bin"));
  ForwardList<int> g;
  ASSERT_TRUE(g.loadBin("/tmp/lab3_f.bin"));
  EXPECT_EQ(g.FLen(), 2);
}

TEST(ForwardList, LoadBadFile) {
  ForwardList<int> f;
  EXPECT_FALSE(f.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(f.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST(List, PushDel) {
  List<int> l;
  l.LPUSHB(1);
  l.LPUSHB(2);
  l.LPUSH(0);
  EXPECT_EQ(l.LLen(), 3);
  EXPECT_TRUE(l.LDEL());
  EXPECT_TRUE(l.LDELBACK());
}

TEST(List, DelVal) {
  List<int> l;
  l.LPUSHB(1);
  l.LPUSHB(2);
  l.LPUSHB(3);
  EXPECT_TRUE(l.LDELVAL(1));
  EXPECT_TRUE(l.LDELVAL(3));
  EXPECT_TRUE(l.LDELVAL(2));
  EXPECT_FALSE(l.LDELVAL(99));
}

TEST(List, DelEmpty) {
  List<int> l;
  EXPECT_FALSE(l.LDEL());
  EXPECT_FALSE(l.LDELBACK());
}

TEST(List, SerializeText) {
  List<int> l;
  l.LPUSHB(10);
  l.LPUSHB(20);
  ASSERT_TRUE(l.saveText("/tmp/lab3_l.txt"));
  List<int> l2;
  ASSERT_TRUE(l2.loadText("/tmp/lab3_l.txt"));
  EXPECT_EQ(l2.LLen(), 2);
}

TEST(List, SerializeBin) {
  List<int> l;
  l.LPUSHB(5);
  l.LPUSHB(6);
  ASSERT_TRUE(l.saveBin("/tmp/lab3_l.bin"));
  List<int> l2;
  ASSERT_TRUE(l2.loadBin("/tmp/lab3_l.bin"));
  EXPECT_EQ(l2.LLen(), 2);
}

TEST(List, LoadBadFile) {
  List<int> l;
  EXPECT_FALSE(l.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(l.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST(Stack, PushPopTop) {
  Stack<int> s;
  int v;
  EXPECT_TRUE(s.SEmpty());
  EXPECT_FALSE(s.STop(v));
  EXPECT_FALSE(s.SPop());
  s.SPush(1);
  s.SPush(2);
  EXPECT_EQ(s.SLen(), 2);
  EXPECT_TRUE(s.STop(v));
  EXPECT_EQ(v, 2);
  EXPECT_TRUE(s.SPop());
  EXPECT_TRUE(s.STop(v));
  EXPECT_EQ(v, 1);
}

TEST(Stack, PopToEmpty) {
  Stack<int> s;
  s.SPush(1);
  EXPECT_TRUE(s.SPop());
  EXPECT_TRUE(s.SEmpty());
  EXPECT_FALSE(s.SPop());
}

TEST(Stack, SerializeText) {
  Stack<int> s;
  s.SPush(1);
  s.SPush(2);
  s.SPush(3);
  ASSERT_TRUE(s.saveText("/tmp/lab3_s.txt"));
  Stack<int> s2;
  ASSERT_TRUE(s2.loadText("/tmp/lab3_s.txt"));
  EXPECT_EQ(s2.SLen(), 3);
  int v;
  ASSERT_TRUE(s2.STop(v));
  EXPECT_EQ(v, 3);
}

TEST(Stack, SerializeBin) {
  Stack<int> s;
  s.SPush(1);
  s.SPush(2);
  s.SPush(3);
  ASSERT_TRUE(s.saveBin("/tmp/lab3_s.bin"));
  Stack<int> s2;
  ASSERT_TRUE(s2.loadBin("/tmp/lab3_s.bin"));
  EXPECT_EQ(s2.SLen(), 3);
  int v;
  ASSERT_TRUE(s2.STop(v));
  EXPECT_EQ(v, 3);
}

TEST(Stack, LoadBadFile) {
  Stack<int> s;
  EXPECT_FALSE(s.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(s.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST(Queue, PushPopFront) {
  Queue<int> q;
  int v;
  EXPECT_TRUE(q.QEmpty());
  EXPECT_FALSE(q.QFront(v));
  EXPECT_FALSE(q.QPop());
  q.QPush(1);
  q.QPush(2);
  EXPECT_EQ(q.QLen(), 2);
  EXPECT_TRUE(q.QFront(v));
  EXPECT_EQ(v, 1);
  EXPECT_TRUE(q.QPop());
  EXPECT_TRUE(q.QFront(v));
  EXPECT_EQ(v, 2);
}

TEST(Queue, PopToEmpty) {
  Queue<int> q;
  q.QPush(1);
  EXPECT_TRUE(q.QPop());
  EXPECT_TRUE(q.QEmpty());
  EXPECT_FALSE(q.QPop());
}

TEST(Queue, SerializeText) {
  Queue<int> q;
  q.QPush(1);
  q.QPush(2);
  q.QPush(3);
  ASSERT_TRUE(q.saveText("/tmp/lab3_q.txt"));
  Queue<int> q2;
  ASSERT_TRUE(q2.loadText("/tmp/lab3_q.txt"));
  EXPECT_EQ(q2.QLen(), 3);
  int v;
  ASSERT_TRUE(q2.QFront(v));
  EXPECT_EQ(v, 1);
}

TEST(Queue, SerializeBin) {
  Queue<int> q;
  q.QPush(1);
  q.QPush(2);
  q.QPush(3);
  ASSERT_TRUE(q.saveBin("/tmp/lab3_q.bin"));
  Queue<int> q2;
  ASSERT_TRUE(q2.loadBin("/tmp/lab3_q.bin"));
  EXPECT_EQ(q2.QLen(), 3);
  int v;
  ASSERT_TRUE(q2.QFront(v));
  EXPECT_EQ(v, 1);
}

TEST(Queue, LoadBadFile) {
  Queue<int> q;
  EXPECT_FALSE(q.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(q.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST(HashTable, InsertFindUpdate) {
  HashTable h;
  string v;
  h.HInsert("one", "1");
  h.HInsert("two", "2");
  EXPECT_EQ(h.HSize(), 2);
  ASSERT_TRUE(h.HFind("one", v));
  EXPECT_EQ(v, "1");
  EXPECT_FALSE(h.HFind("nope", v));
  h.HInsert("one", "11");
  ASSERT_TRUE(h.HFind("one", v));
  EXPECT_EQ(v, "11");
}

TEST(HashTable, Delete) {
  HashTable h;
  h.HInsert("a", "A");
  h.HInsert("k", "K");
  EXPECT_TRUE(h.HDelete("a"));
  EXPECT_TRUE(h.HDelete("k"));
  EXPECT_FALSE(h.HDelete("nope"));
}

TEST(HashTable, Collisions) {
  HashTable h;
  string v;
  h.HInsert("a", "A");
  h.HInsert("k", "K");
  ASSERT_TRUE(h.HFind("a", v));
  EXPECT_EQ(v, "A");
  ASSERT_TRUE(h.HFind("k", v));
  EXPECT_EQ(v, "K");
  EXPECT_EQ(h.HSize(), 2);
}

TEST(HashTable, SerializeText) {
  HashTable h;
  h.HInsert("x", "1");
  h.HInsert("y", "2");
  ASSERT_TRUE(h.saveText("/tmp/lab3_h.txt"));
  HashTable h2;
  ASSERT_TRUE(h2.loadText("/tmp/lab3_h.txt"));
  EXPECT_EQ(h2.HSize(), 2);
  string v;
  ASSERT_TRUE(h2.HFind("x", v));
  EXPECT_EQ(v, "1");
}

TEST(HashTable, SerializeBin) {
  HashTable h;
  h.HInsert("x", "1");
  h.HInsert("yy", "22");
  ASSERT_TRUE(h.saveBin("/tmp/lab3_h.bin"));
  HashTable h2;
  ASSERT_TRUE(h2.loadBin("/tmp/lab3_h.bin"));
  EXPECT_EQ(h2.HSize(), 2);
  string v;
  ASSERT_TRUE(h2.HFind("yy", v));
  EXPECT_EQ(v, "22");
}

TEST(HashTable, LoadBadFile) {
  HashTable h;
  EXPECT_FALSE(h.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(h.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

TEST(Tree, InsertFind) {
  CompleteBinaryTree<int> t;
  EXPECT_FALSE(t.TFind(1));
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  EXPECT_EQ(t.TSize(), 7);
  for (int i = 1; i <= 7; i++)
    EXPECT_TRUE(t.TFind(i));
  EXPECT_FALSE(t.TFind(99));
}

TEST(Tree, IsFull) {
  CompleteBinaryTree<int> t;
  EXPECT_TRUE(t.TIsFull());
  t.TInsert(1);
  EXPECT_TRUE(t.TIsFull());
  t.TInsert(2);
  EXPECT_FALSE(t.TIsFull());
  t.TInsert(3);
  EXPECT_TRUE(t.TIsFull());
  for (int i = 4; i <= 7; i++)
    t.TInsert(i);
  EXPECT_TRUE(t.TIsFull());
  t.TInsert(8);
  EXPECT_FALSE(t.TIsFull());
}

TEST(Tree, ClearReuse) {
  CompleteBinaryTree<int> t;
  t.TInsert(1);
  t.TInsert(2);
  t.clear();
  EXPECT_EQ(t.TSize(), 0);
  EXPECT_TRUE(t.TInsert(42));
  EXPECT_TRUE(t.TFind(42));
}

TEST(Tree, SerializeText) {
  CompleteBinaryTree<int> t;
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  ASSERT_TRUE(t.saveText("/tmp/lab3_t.txt"));
  CompleteBinaryTree<int> t2;
  ASSERT_TRUE(t2.loadText("/tmp/lab3_t.txt"));
  EXPECT_EQ(t2.TSize(), 7);
  EXPECT_TRUE(t2.TIsFull());
}

TEST(Tree, SerializeBin) {
  CompleteBinaryTree<int> t;
  for (int i = 1; i <= 7; i++)
    t.TInsert(i);
  ASSERT_TRUE(t.saveBin("/tmp/lab3_t.bin"));
  CompleteBinaryTree<int> t2;
  ASSERT_TRUE(t2.loadBin("/tmp/lab3_t.bin"));
  EXPECT_EQ(t2.TSize(), 7);
  EXPECT_TRUE(t2.TFind(7));
}

TEST(Tree, LoadBadFile) {
  CompleteBinaryTree<int> t;
  EXPECT_FALSE(t.loadText("/tmp/lab3_nonexistent_xyz.txt"));
  EXPECT_FALSE(t.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
