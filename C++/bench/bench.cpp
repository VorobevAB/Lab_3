#include "struct/Array.h"
#include "struct/CompleteBinaryTree.h"
#include "struct/ForwardList.h"
#include "struct/HashTable.h"
#include "struct/List.h"
#include "struct/Queue.h"
#include "struct/Stack.h"
#include <benchmark/benchmark.h>
#include <string>

static void BM_ArrayPush(benchmark::State &s) {
  for (auto _ : s) {
    Array<int> a;
    for (int i = 0; i < s.range(0); i++)
      a.MPush(i);
    benchmark::DoNotOptimize(a);
  }
}
BENCHMARK(BM_ArrayPush)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_ArrayPushIdx(benchmark::State &s) {
  for (auto _ : s) {
    Array<int> a;
    for (int i = 0; i < s.range(0); i++)
      a.MPush(i);
    benchmark::DoNotOptimize(a);
  }
}
BENCHMARK(BM_ArrayPushIdx)
    ->Arg(1000)
    ->Arg(10000)
    ->Unit(benchmark::kMicrosecond);

static void BM_ForwardListPush(benchmark::State &s) {
  for (auto _ : s) {
    ForwardList<int> f;
    for (int i = 0; i < s.range(0); i++)
      f.FPUSH(i);
    benchmark::DoNotOptimize(f);
  }
}
BENCHMARK(BM_ForwardListPush)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_ForwardListPushBack(benchmark::State &s) {
  for (auto _ : s) {
    ForwardList<int> f;
    for (int i = 0; i < s.range(0); i++)
      f.FPUSHB(i);
    benchmark::DoNotOptimize(f);
  }
}
BENCHMARK(BM_ForwardListPushBack)
    ->Arg(1000)
    ->Arg(10000)
    ->Unit(benchmark::kMicrosecond);

static void BM_ForwardListFind(benchmark::State &s) {
  ForwardList<int> f;
  for (int i = 0; i < s.range(0); i++)
    f.FPUSHB(i);
  for (auto _ : s) {
    benchmark::DoNotOptimize(f.FFIND(s.range(0) - 1));
  }
}
BENCHMARK(BM_ForwardListFind)
    ->Arg(1000)
    ->Arg(10000)
    ->Unit(benchmark::kMicrosecond);

static void BM_ListPushBack(benchmark::State &s) {
  for (auto _ : s) {
    List<int> l;
    for (int i = 0; i < s.range(0); i++)
      l.LPUSHB(i);
    benchmark::DoNotOptimize(l);
  }
}
BENCHMARK(BM_ListPushBack)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_ListPushFront(benchmark::State &s) {
  for (auto _ : s) {
    List<int> l;
    for (int i = 0; i < s.range(0); i++)
      l.LPUSH(i);
    benchmark::DoNotOptimize(l);
  }
}
BENCHMARK(BM_ListPushFront)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_StackPush(benchmark::State &s) {
  for (auto _ : s) {
    Stack<int> st;
    for (int i = 0; i < s.range(0); i++)
      st.SPush(i);
    benchmark::DoNotOptimize(st);
  }
}
BENCHMARK(BM_StackPush)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_StackPushPop(benchmark::State &s) {
  for (auto _ : s) {
    Stack<int> st;
    for (int i = 0; i < s.range(0); i++)
      st.SPush(i);
    for (int i = 0; i < s.range(0); i++)
      st.SPop();
    benchmark::DoNotOptimize(st);
  }
}
BENCHMARK(BM_StackPushPop)
    ->Arg(1000)
    ->Arg(10000)
    ->Unit(benchmark::kMicrosecond);

static void BM_QueuePush(benchmark::State &s) {
  for (auto _ : s) {
    Queue<int> q;
    for (int i = 0; i < s.range(0); i++)
      q.QPush(i);
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK(BM_QueuePush)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_QueuePushPop(benchmark::State &s) {
  for (auto _ : s) {
    Queue<int> q;
    for (int i = 0; i < s.range(0); i++)
      q.QPush(i);
    for (int i = 0; i < s.range(0); i++)
      q.QPop();
    benchmark::DoNotOptimize(q);
  }
}
BENCHMARK(BM_QueuePushPop)
    ->Arg(1000)
    ->Arg(10000)
    ->Unit(benchmark::kMicrosecond);

static void BM_HashInsert(benchmark::State &s) {
  for (auto _ : s) {
    HashTable h;
    for (int i = 0; i < s.range(0); i++)
      h.HInsert("key" + std::to_string(i), "v");
    benchmark::DoNotOptimize(h);
  }
}
BENCHMARK(BM_HashInsert)->Arg(1000)->Arg(10000)->Unit(benchmark::kMicrosecond);

static void BM_HashFind(benchmark::State &s) {
  HashTable h;
  for (int i = 0; i < s.range(0); i++)
    h.HInsert("key" + std::to_string(i), "v");
  std::string v;
  for (auto _ : s) {
    benchmark::DoNotOptimize(
        h.HFind("key" + std::to_string(s.range(0) - 1), v));
  }
}
BENCHMARK(BM_HashFind)->Arg(1000)->Arg(10000)->Unit(benchmark::kMicrosecond);

static void BM_TreeInsert(benchmark::State &s) {
  for (auto _ : s) {
    CompleteBinaryTree<int> t;
    for (int i = 0; i < s.range(0); i++)
      t.TInsert(i);
    benchmark::DoNotOptimize(t);
  }
}
BENCHMARK(BM_TreeInsert)
    ->Arg(1000)
    ->Arg(10000)
    ->Arg(100000)
    ->Unit(benchmark::kMicrosecond);

static void BM_TreeFind(benchmark::State &s) {
  CompleteBinaryTree<int> t;
  for (int i = 0; i < s.range(0); i++)
    t.TInsert(i);
  for (auto _ : s) {
    benchmark::DoNotOptimize(t.TFind(s.range(0) - 1));
  }
}
BENCHMARK(BM_TreeFind)->Arg(1000)->Arg(10000)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
