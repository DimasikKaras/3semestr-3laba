#!/bin/bash
echo "=== БЕЗОПАСНЫЙ ЗАПУСК БЕНЧМАРКОВ ==="

cd "$(dirname "$0")"

# Создаем build директорию если ее нет
if [ ! -d "build" ]; then
    mkdir build
fi

cd build

# Конфигурация и сборка
echo "=== СБОРКА ПРОЕКТА ==="
cmake -DCMAKE_BUILD_TYPE=Release ..
make benchmarks -j$(nproc)

# Запуск бенчмарков по одному для стабильности
echo "=== ЗАПУСК БЕНЧМАРКОВ (безопасный режим) ==="

# Запускаем каждый бенчмарк отдельно
echo "1. Тестируем динамический массив..."
./benchmarks --benchmark_filter="BM_DyArr" --benchmark_format=console

echo "2. Тестируем односвязный список..."
./benchmarks --benchmark_filter="BM_SimplyList" --benchmark_format=console

echo "3. Тестируем двусвязный список..."
./benchmarks --benchmark_filter="BM_DoublyList" --benchmark_format=console

echo "4. Тестируем стек..."
./benchmarks --benchmark_filter="BM_Stack" --benchmark_format=console

echo "5. Тестируем очередь..."
./benchmarks --benchmark_filter="BM_Queue" --benchmark_format=console

echo "6. Тестируем AVL дерево..."
./benchmarks --benchmark_filter="BM_AVLTree" --benchmark_format=console

echo "7. Тестируем хэш-таблицы..."
./benchmarks --benchmark_filter="BM_.*HashTable" --benchmark_format=console

echo "=== ВСЕ БЕНЧМАРКИ ЗАВЕРШЕНЫ ==="
