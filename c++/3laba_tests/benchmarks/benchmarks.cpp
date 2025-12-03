#include <benchmark/benchmark.h>
#include <iostream>
#include <string>
#include <vector>
#include <random>

// Подключаем наши структуры данных
#include "dyArr.h"
#include "simplyList.h"
#include "doublyList.h"
#include "stack.h"
#include "queue.h"
#include "avlTree.h"
#include "HashTable.h"

using namespace std;

// Безопасный генератор данных
class DataGenerator {
public:
    static vector<string> generateStrings(int count) {
        vector<string> result;
        for (int i = 0; i < count; i++) {
            result.push_back("test_" + to_string(i));
        }
        return result;
    }
    
    static vector<int> generateIntegers(int count) {
        vector<int> result;
        for (int i = 0; i < count; i++) {
            result.push_back(i + 1);
        }
        return result;
    }
};

// ==================== БЕЗОПАСНЫЕ БЕНЧМАРКИ ====================

static void BM_DyArr_PushBack(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        dyArr arr(10);
        for (int i = 0; i < size; i++) {
            arr.pushBack("element_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_DyArr_GetElement(benchmark::State& state) {
    dyArr arr(100);
    for (int i = 0; i < 100; i++) {
        arr.pushBack("element_" + to_string(i));
    }
    
    for (auto _ : state) {
        auto result = arr.getElement(50);
        benchmark::DoNotOptimize(result);
    }
}

static void BM_SimplyList_AddHead(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        SimplyList list;
        for (int i = 0; i < size; i++) {
            list.addHead("item_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_SimplyList_AddBack(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        SimplyList list;
        for (int i = 0; i < size; i++) {
            list.addBack("item_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_DoublyList_AddHead(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        DoublyList list;
        for (int i = 0; i < size; i++) {
            list.addHead("item_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_DoublyList_AddBack(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        DoublyList list;
        for (int i = 0; i < size; i++) {
            list.addBack("item_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_Stack_Push(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        Stack stack;
        for (int i = 0; i < size; i++) {
            stack.push("item_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_Stack_PushPop(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        Stack stack;
        for (int i = 0; i < size; i++) {
            stack.push("item_" + to_string(i));
        }
        for (int i = 0; i < size; i++) {
            auto result = stack.pop();
            benchmark::DoNotOptimize(result);
        }
    }
    state.SetComplexityN(size);
}

static void BM_Queue_Enqueue(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        Queue queue;
        for (int i = 0; i < size; i++) {
            queue.enqueue("item_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_Queue_EnqueueDequeue(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        Queue queue;
        for (int i = 0; i < size; i++) {
            queue.enqueue("item_" + to_string(i));
        }
        for (int i = 0; i < size; i++) {
            auto result = queue.dequeue();
            benchmark::DoNotOptimize(result);
        }
    }
    state.SetComplexityN(size);
}

static void BM_AVLTree_Insert(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        AVLTree tree;
        for (int i = 0; i < size; i++) {
            tree.insert(i);
        }
    }
    state.SetComplexityN(size);
}

static void BM_AVLTree_Search(benchmark::State& state) {
    AVLTree tree;
    for (int i = 0; i < 1000; i++) {
        tree.insert(i);
    }
    
    for (auto _ : state) {
        auto result = tree.contains(500);
        benchmark::DoNotOptimize(result);
    }
}

static void BM_OpenHashTable_Insert(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        OpenAddressingHashTable table(1000);
        for (int i = 0; i < size; i++) {
            table.insert("key_" + to_string(i), "value_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

static void BM_ChainHashTable_Insert(benchmark::State& state) {
    int size = state.range(0);
    for (auto _ : state) {
        ChainHashTable table(1000);
        for (int i = 0; i < size; i++) {
            table.insert("key_" + to_string(i), "value_" + to_string(i));
        }
    }
    state.SetComplexityN(size);
}

// ==================== РЕГИСТРАЦИЯ БЕНЧМАРКОВ ====================

// Базовые бенчмарки с малыми размерами для стабильности
BENCHMARK(BM_DyArr_PushBack)->Range(8, 512)->Complexity();
BENCHMARK(BM_DyArr_GetElement);

BENCHMARK(BM_SimplyList_AddHead)->Range(8, 512)->Complexity();
BENCHMARK(BM_SimplyList_AddBack)->Range(8, 512)->Complexity();

BENCHMARK(BM_DoublyList_AddHead)->Range(8, 512)->Complexity();
BENCHMARK(BM_DoublyList_AddBack)->Range(8, 512)->Complexity();

BENCHMARK(BM_Stack_Push)->Range(8, 512)->Complexity();
BENCHMARK(BM_Stack_PushPop)->Range(8, 256)->Complexity();

BENCHMARK(BM_Queue_Enqueue)->Range(8, 512)->Complexity();
BENCHMARK(BM_Queue_EnqueueDequeue)->Range(8, 256)->Complexity();

BENCHMARK(BM_AVLTree_Insert)->Range(8, 512)->Complexity();
BENCHMARK(BM_AVLTree_Search);

BENCHMARK(BM_OpenHashTable_Insert)->Range(8, 512)->Complexity();
BENCHMARK(BM_ChainHashTable_Insert)->Range(8, 512)->Complexity();

BENCHMARK_MAIN();
