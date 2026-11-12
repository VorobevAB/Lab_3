#define BOOST_TEST_MODULE Lab3
#include <boost/test/included/unit_test.hpp>
#include "struct/Array.h"
#include "struct/ForwardList.h"
#include "struct/List.h"
#include "struct/Stack.h"
#include "struct/Queue.h"
#include "struct/HashTable.h"
#include "struct/CompleteBinaryTree.h"

BOOST_AUTO_TEST_SUITE(Lab3Suite)

BOOST_AUTO_TEST_CASE(array_push_get_len) {
    Array<int> a;
    a.MPush(10);
    a.MPush(30);
    a.MPushIdx(1, 20);
    BOOST_TEST(a.MLen() == 3);
    int v;
    BOOST_TEST(a.MGet(1, v));
    BOOST_TEST(v == 20);
}

BOOST_AUTO_TEST_CASE(array_push_idx_errors) {
    Array<int> a;
    a.MPush(1);
    BOOST_TEST(!a.MPushIdx(-1, 0));
    BOOST_TEST(!a.MPushIdx(5, 0));
    BOOST_TEST(a.MPushIdx(0, 9));
    BOOST_TEST(a.MPushIdx(a.MLen(), 8));
}

BOOST_AUTO_TEST_CASE(array_get_set_del_errors) {
    Array<int> a;
    int v;
    BOOST_TEST(!a.MGet(0, v));
    BOOST_TEST(!a.MSetInd(0, 1));
    BOOST_TEST(!a.MDelInd(0));
}

BOOST_AUTO_TEST_CASE(array_del_clear) {
    Array<int> a;
    a.MPush(1);
    a.MPush(2);
    a.MPush(3);
    BOOST_TEST(a.MDelInd(0));
    int v;
    BOOST_TEST(a.MGet(0, v));
    BOOST_TEST(v == 2);
    a.clear();
    BOOST_TEST(a.MLen() == 0);
    BOOST_TEST(a.MPush(5));
    BOOST_TEST(a.MLen() == 1);
}

BOOST_AUTO_TEST_CASE(array_serialize_text) {
    Array<int> a;
    a.MPush(1);
    a.MPush(2);
    BOOST_TEST(a.saveText("/tmp/lab3_b_a.txt"));
    Array<int> b;
    BOOST_TEST(b.loadText("/tmp/lab3_b_a.txt"));
    BOOST_TEST(b.MLen() == 2);
}

BOOST_AUTO_TEST_CASE(array_serialize_bin) {
    Array<int> a;
    a.MPush(7);
    a.MPush(8);
    BOOST_TEST(a.saveBin("/tmp/lab3_b_a.bin"));
    Array<int> b;
    BOOST_TEST(b.loadBin("/tmp/lab3_b_a.bin"));
    BOOST_TEST(b.MLen() == 2);
}

BOOST_AUTO_TEST_CASE(array_load_bad_file) {
    Array<int> a;
    BOOST_TEST(!a.loadText("/tmp/lab3_nonexistent_xyz.txt"));
    BOOST_TEST(!a.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

BOOST_AUTO_TEST_CASE(flist_push_find_del) {
    ForwardList<int> f;
    f.FPUSH(2);
    f.FPUSH(1);
    f.FPUSHB(3);
    BOOST_TEST(f.FLen() == 3);
    BOOST_TEST(f.FFIND(3));
    BOOST_TEST(f.FDELVAL(2));
    BOOST_TEST(!f.FFIND(2));
    BOOST_TEST(!f.FDELVAL(99));
}

BOOST_AUTO_TEST_CASE(flist_del_empty) {
    ForwardList<int> f;
    BOOST_TEST(!f.FDEL());
}

BOOST_AUTO_TEST_CASE(flist_del_head_tail) {
    ForwardList<int> f;
    f.FPUSHB(1);
    f.FPUSHB(2);
    f.FPUSHB(3);
    BOOST_TEST(f.FDEL());
    BOOST_TEST(!f.FFIND(1));
    BOOST_TEST(f.FDELVAL(3));
    BOOST_TEST(!f.FFIND(3));
}

BOOST_AUTO_TEST_CASE(flist_serialize_text) {
    ForwardList<int> f;
    f.FPUSHB(1);
    f.FPUSHB(2);
    BOOST_TEST(f.saveText("/tmp/lab3_b_f.txt"));
    ForwardList<int> g;
    BOOST_TEST(g.loadText("/tmp/lab3_b_f.txt"));
    BOOST_TEST(g.FLen() == 2);
}

BOOST_AUTO_TEST_CASE(flist_serialize_bin) {
    ForwardList<int> f;
    f.FPUSHB(7);
    f.FPUSHB(8);
    BOOST_TEST(f.saveBin("/tmp/lab3_b_f.bin"));
    ForwardList<int> g;
    BOOST_TEST(g.loadBin("/tmp/lab3_b_f.bin"));
    BOOST_TEST(g.FLen() == 2);
}

BOOST_AUTO_TEST_CASE(flist_load_bad_file) {
    ForwardList<int> f;
    BOOST_TEST(!f.loadText("/tmp/lab3_nonexistent_xyz.txt"));
    BOOST_TEST(!f.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

BOOST_AUTO_TEST_CASE(list_push_del) {
    List<int> l;
    l.LPUSHB(1);
    l.LPUSHB(2);
    l.LPUSH(0);
    BOOST_TEST(l.LLen() == 3);
    BOOST_TEST(l.LDEL());
    BOOST_TEST(l.LDELBACK());
}

BOOST_AUTO_TEST_CASE(list_del_val) {
    List<int> l;
    l.LPUSHB(1);
    l.LPUSHB(2);
    l.LPUSHB(3);
    BOOST_TEST(l.LDELVAL(1));
    BOOST_TEST(l.LDELVAL(3));
    BOOST_TEST(l.LDELVAL(2));
    BOOST_TEST(!l.LDELVAL(99));
}

BOOST_AUTO_TEST_CASE(list_del_empty) {
    List<int> l;
    BOOST_TEST(!l.LDEL());
    BOOST_TEST(!l.LDELBACK());
}

BOOST_AUTO_TEST_CASE(list_serialize_text) {
    List<int> l;
    l.LPUSHB(10);
    l.LPUSHB(20);
    BOOST_TEST(l.saveText("/tmp/lab3_b_l.txt"));
    List<int> l2;
    BOOST_TEST(l2.loadText("/tmp/lab3_b_l.txt"));
    BOOST_TEST(l2.LLen() == 2);
}

BOOST_AUTO_TEST_CASE(list_serialize_bin) {
    List<int> l;
    l.LPUSHB(5);
    l.LPUSHB(6);
    BOOST_TEST(l.saveBin("/tmp/lab3_b_l.bin"));
    List<int> l2;
    BOOST_TEST(l2.loadBin("/tmp/lab3_b_l.bin"));
    BOOST_TEST(l2.LLen() == 2);
}

BOOST_AUTO_TEST_CASE(list_load_bad_file) {
    List<int> l;
    BOOST_TEST(!l.loadText("/tmp/lab3_nonexistent_xyz.txt"));
    BOOST_TEST(!l.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

BOOST_AUTO_TEST_CASE(stack_push_pop_top) {
    Stack<int> s;
    int v;
    BOOST_TEST(s.SEmpty());
    BOOST_TEST(!s.STop(v));
    BOOST_TEST(!s.SPop());
    s.SPush(1);
    s.SPush(2);
    BOOST_TEST(s.SLen() == 2);
    BOOST_TEST(s.STop(v));
    BOOST_TEST(v == 2);
    BOOST_TEST(s.SPop());
}

BOOST_AUTO_TEST_CASE(stack_pop_to_empty) {
    Stack<int> s;
    s.SPush(1);
    BOOST_TEST(s.SPop());
    BOOST_TEST(s.SEmpty());
    BOOST_TEST(!s.SPop());
}

BOOST_AUTO_TEST_CASE(stack_serialize_text) {
    Stack<int> s;
    s.SPush(1);
    s.SPush(2);
    BOOST_TEST(s.saveText("/tmp/lab3_b_s.txt"));
    Stack<int> s2;
    BOOST_TEST(s2.loadText("/tmp/lab3_b_s.txt"));
    BOOST_TEST(s2.SLen() == 2);
    int v;
    BOOST_TEST(s2.STop(v));
    BOOST_TEST(v == 2);
}

BOOST_AUTO_TEST_CASE(stack_serialize_bin) {
    Stack<int> s;
    s.SPush(1);
    s.SPush(2);
    BOOST_TEST(s.saveBin("/tmp/lab3_b_s.bin"));
    Stack<int> s2;
    BOOST_TEST(s2.loadBin("/tmp/lab3_b_s.bin"));
    BOOST_TEST(s2.SLen() == 2);
}

BOOST_AUTO_TEST_CASE(stack_load_bad_file) {
    Stack<int> s;
    BOOST_TEST(!s.loadText("/tmp/lab3_nonexistent_xyz.txt"));
    BOOST_TEST(!s.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

BOOST_AUTO_TEST_CASE(queue_push_pop_front) {
    Queue<int> q;
    int v;
    BOOST_TEST(q.QEmpty());
    BOOST_TEST(!q.QFront(v));
    BOOST_TEST(!q.QPop());
    q.QPush(1);
    q.QPush(2);
    BOOST_TEST(q.QLen() == 2);
    BOOST_TEST(q.QFront(v));
    BOOST_TEST(v == 1);
}

BOOST_AUTO_TEST_CASE(queue_pop_to_empty) {
    Queue<int> q;
    q.QPush(1);
    BOOST_TEST(q.QPop());
    BOOST_TEST(q.QEmpty());
    BOOST_TEST(!q.QPop());
}

BOOST_AUTO_TEST_CASE(queue_serialize_text) {
    Queue<int> q;
    q.QPush(1);
    q.QPush(2);
    BOOST_TEST(q.saveText("/tmp/lab3_b_q.txt"));
    Queue<int> q2;
    BOOST_TEST(q2.loadText("/tmp/lab3_b_q.txt"));
    BOOST_TEST(q2.QLen() == 2);
    int v;
    BOOST_TEST(q2.QFront(v));
    BOOST_TEST(v == 1);
}

BOOST_AUTO_TEST_CASE(queue_serialize_bin) {
    Queue<int> q;
    q.QPush(1);
    q.QPush(2);
    BOOST_TEST(q.saveBin("/tmp/lab3_b_q.bin"));
    Queue<int> q2;
    BOOST_TEST(q2.loadBin("/tmp/lab3_b_q.bin"));
    BOOST_TEST(q2.QLen() == 2);
}

BOOST_AUTO_TEST_CASE(queue_load_bad_file) {
    Queue<int> q;
    BOOST_TEST(!q.loadText("/tmp/lab3_nonexistent_xyz.txt"));
    BOOST_TEST(!q.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

BOOST_AUTO_TEST_CASE(hash_insert_find) {
    HashTable h;
    string v;
    h.HInsert("one", "1");
    h.HInsert("two", "2");
    BOOST_TEST(h.HSize() == 2);
    BOOST_TEST(h.HFind("one", v));
    BOOST_TEST(v == "1");
    BOOST_TEST(!h.HFind("nope", v));
}

BOOST_AUTO_TEST_CASE(hash_update) {
    HashTable h;
    string v;
    h.HInsert("one", "1");
    h.HInsert("one", "11");
    BOOST_TEST(h.HFind("one", v));
    BOOST_TEST(v == "11");
}

BOOST_AUTO_TEST_CASE(hash_delete) {
    HashTable h;
    h.HInsert("a", "A");
    h.HInsert("k", "K");
    BOOST_TEST(h.HDelete("a"));
    BOOST_TEST(h.HDelete("k"));
    BOOST_TEST(!h.HDelete("nope"));
}

BOOST_AUTO_TEST_CASE(hash_collisions) {
    HashTable h;
    string v;
    h.HInsert("a", "A");
    h.HInsert("k", "K");
    BOOST_TEST(h.HFind("a", v));
    BOOST_TEST(v == "A");
    BOOST_TEST(h.HFind("k", v));
    BOOST_TEST(v == "K");
}

BOOST_AUTO_TEST_CASE(hash_serialize_text) {
    HashTable h;
    h.HInsert("x", "1");
    h.HInsert("y", "2");
    BOOST_TEST(h.saveText("/tmp/lab3_b_h.txt"));
    HashTable h2;
    BOOST_TEST(h2.loadText("/tmp/lab3_b_h.txt"));
    BOOST_TEST(h2.HSize() == 2);
}

BOOST_AUTO_TEST_CASE(hash_serialize_bin) {
    HashTable h;
    h.HInsert("x", "1");
    h.HInsert("yy", "22");
    BOOST_TEST(h.saveBin("/tmp/lab3_b_h.bin"));
    HashTable h2;
    BOOST_TEST(h2.loadBin("/tmp/lab3_b_h.bin"));
    BOOST_TEST(h2.HSize() == 2);
}

BOOST_AUTO_TEST_CASE(hash_load_bad_file) {
    HashTable h;
    BOOST_TEST(!h.loadText("/tmp/lab3_nonexistent_xyz.txt"));
    BOOST_TEST(!h.loadBin("/tmp/lab3_nonexistent_xyz.bin"));
}

BOOST_AUTO_TEST_CASE(tree_insert_find) {
    CompleteBinaryTree<int> t;
    BOOST_TEST(!t.TFind(1));
    for (int i = 1; i <= 7; i++)
        t.TInsert(i);
    BOOST_TEST(t.TSize() == 7);
    BOOST_TEST(t.TFind(1));
    BOOST_TEST(t.TFind(7));
}

BOOST_AUTO_TEST_CASE(tree_is_full) {
    CompleteBinaryTree<int> t;
    BOOST_TEST(t.TIsFull());
    t.TInsert(1);
    BOOST_TEST(t.TIsFull());
    t.TInsert(2);
    BOOST_TEST(!t.TIsFull());
    t.TInsert(3);
    BOOST_TEST(t.TIsFull());
}

BOOST_AUTO_TEST_CASE(tree_serialize_text) {
    CompleteBinaryTree<int> t;
    for (int i = 1; i <= 7; i++)
        t.TInsert(i);
    BOOST_TEST(t.saveText("/tmp/lab3_b_t.txt"));
    CompleteBinaryTree<int> t2;
    BOOST_TEST(t2.loadText("/tmp/lab3_b_t.txt"));
    BOOST_TEST(t2.TSize() == 7);
}

BOOST_AUTO_TEST_CASE(tree_serialize_bin) {
    CompleteBinaryTree<int> t;
    for (int i = 1; i <= 7; i++)
        t.TInsert(i);
    BOOST_TEST(t.saveBin("/tmp/lab3_b_t.bin"));
    CompleteBinaryTree<int> t2;
    BOOST_TEST(t2.loadBin("/tmp/lab3_b_t.bin"));
    BOOST_TEST(t2.TSize() == 7);
}

BOOST_AUTO_TEST_CASE(tree_clear_reuse) {
    CompleteBinaryTree<int> t;
    t.TInsert(1);
    t.TInsert(2);
    t.clear();
    BOOST_TEST(t.TSize() == 0);
    BOOST_TEST(t.TInsert(42));
    BOOST_TEST(t.TFind(42));
}

BOOST_AUTO_TEST_SUITE_END()
