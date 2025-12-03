use criterion::{
    black_box, criterion_group, criterion_main, Criterion,
};
use laba3_rust::structures::{
    DyArr, SimplyList, DoublyList, Stack, Queue,
    OpenAddressingHashTable, ChainHashTable
};

// Бенчмарки для массива
fn bench_array_operations(c: &mut Criterion) {
    let mut group = c.benchmark_group("Array Operations");

    group.bench_function("array_push_back_1000", |b| {
        b.iter(|| {
            let mut arr = DyArr::new(1000);
            for i in 0..1000 {
                arr.push_back(black_box(&format!("value_{}", i)));
            }
            black_box(arr);
        });
    });

    group.bench_function("array_insert_at_index", |b| {
        b.iter(|| {
            let mut arr = DyArr::new(100);
            for i in 0..100 {
                arr.push_back(black_box(&format!("value_{}", i)));
            }
            arr.push_in_index(black_box("middle"), black_box(50)).unwrap();
            black_box(arr);
        });
    });

    group.bench_function("array_get_element", |b| {
        let mut arr = DyArr::new(1000);
        for i in 0..1000 {
            arr.push_back(&format!("value_{}", i));
        }

        b.iter(|| {
            for i in 0..1000 {
                black_box(arr.get_element(black_box(i % 1000)).unwrap());
            }
        });
    });

    group.finish();
}

// Бенчмарки для односвязного списка
fn bench_simply_list_operations(c: &mut Criterion) {
    let mut group = c.benchmark_group("SimplyList Operations");

    group.bench_function("simply_list_add_head_1000", |b| {
        b.iter(|| {
            let mut list = SimplyList::new();
            for i in 0..1000 {
                list.add_head(black_box(&format!("value_{}", i))).unwrap();
            }
            black_box(list);
        });
    });

    group.bench_function("simply_list_add_tail_1000", |b| {
        b.iter(|| {
            let mut list = SimplyList::new();
            for i in 0..1000 {
                list.add_back(black_box(&format!("value_{}", i))).unwrap();
            }
            black_box(list);
        });
    });

    group.bench_function("simply_list_search", |b| {
        let mut list = SimplyList::new();
        for i in 0..1000 {
            list.add_back(&format!("value_{}", i)).unwrap();
        }

        b.iter(|| {
            black_box(list.search_value(black_box("value_500")).unwrap());
        });
    });

    group.finish();
}

// Бенчмарки для двусвязного списка
fn bench_doubly_list_operations(c: &mut Criterion) {
    let mut group = c.benchmark_group("DoublyList Operations");

    group.bench_function("doubly_list_add_head_1000", |b| {
        b.iter(|| {
            let mut list = DoublyList::new();
            for i in 0..1000 {
                list.add_head(black_box(&format!("value_{}", i))).unwrap();
            }
            black_box(list);
        });
    });

    group.bench_function("doubly_list_add_tail_1000", |b| {
        b.iter(|| {
            let mut list = DoublyList::new();
            for i in 0..1000 {
                list.add_back(black_box(&format!("value_{}", i))).unwrap();
            }
            black_box(list);
        });
    });

    group.bench_function("doubly_list_insert_middle", |b| {
        b.iter(|| {
            let mut list = DoublyList::new();
            for i in 0..100 {
                list.add_back(&format!("value_{}", i)).unwrap();
            }
            list.add_after_index(black_box(50), black_box("middle")).unwrap();
            black_box(list);
        });
    });

    group.finish();
}

// Бенчмарки для стека
fn bench_stack_operations(c: &mut Criterion) {
    let mut group = c.benchmark_group("Stack Operations");

    group.bench_function("stack_push_1000", |b| {
        b.iter(|| {
            let mut stack = Stack::new();
            for i in 0..1000 {
                stack.push(black_box(&format!("value_{}", i)));
            }
            black_box(stack);
        });
    });

    group.bench_function("stack_push_pop", |b| {
        b.iter(|| {
            let mut stack = Stack::new();
            for i in 0..1000 {
                stack.push(black_box(&format!("value_{}", i)));
            }
            for _ in 0..1000 {
                black_box(stack.pop().unwrap());
            }
        });
    });

    group.finish();
}

// Бенчмарки для очереди
fn bench_queue_operations(c: &mut Criterion) {
    let mut group = c.benchmark_group("Queue Operations");

    group.bench_function("queue_enqueue_1000", |b| {
        b.iter(|| {
            let mut queue = Queue::new();
            for i in 0..1000 {
                queue.enqueue(black_box(&format!("value_{}", i)));
            }
            black_box(queue);
        });
    });

    group.bench_function("queue_enqueue_dequeue", |b| {
        b.iter(|| {
            let mut queue = Queue::new();
            for i in 0..1000 {
                queue.enqueue(black_box(&format!("value_{}", i)));
            }
            for _ in 0..1000 {
                black_box(queue.dequeue().unwrap());
            }
        });
    });

    group.finish();
}

// Бенчмарки для хэш-таблиц
fn bench_hash_table_operations(c: &mut Criterion) {
    let mut group = c.benchmark_group("HashTable Operations");

    // Открытая адресация
    group.bench_function("open_hash_insert_1000", |b| {
        b.iter(|| {
            let mut table = OpenAddressingHashTable::new(2000);
            for i in 0..1000 {
                table.insert(
                    black_box(&format!("key_{}", i)),
                    black_box(&format!("value_{}", i))
                ).unwrap();
            }
            black_box(table);
        });
    });

    group.bench_function("open_hash_search", |b| {
        let mut table = OpenAddressingHashTable::new(2000);
        for i in 0..1000 {
            table.insert(&format!("key_{}", i), &format!("value_{}", i)).unwrap();
        }

        b.iter(|| {
            for i in 0..100 {
                black_box(table.search(black_box("key_500")).unwrap());
            }
        });
    });

    // Метод цепочек
    group.bench_function("chain_hash_insert_1000", |b| {
        b.iter(|| {
            let mut table = ChainHashTable::new(2000);
            for i in 0..1000 {
                table.insert(
                    black_box(&format!("key_{}", i)),
                    black_box(&format!("value_{}", i))
                ).unwrap();
            }
            black_box(table);
        });
    });

    group.bench_function("chain_hash_search", |b| {
        let mut table = ChainHashTable::new(2000);
        for i in 0..1000 {
            table.insert(&format!("key_{}", i), &format!("value_{}", i)).unwrap();
        }

        b.iter(|| {
            for i in 0..100 {
                black_box(table.search(black_box("key_500")).unwrap());
            }
        });
    });

    group.finish();
}

// Сравнительные бенчмарки
fn bench_comparison(c: &mut Criterion) {
    let mut group = c.benchmark_group("Comparison");

    // Сравнение вставки в разные структуры
    group.bench_function("array_vs_list_insert_1000", |b| {
        b.iter(|| {
            // Массив
            let mut arr = DyArr::new(1000);
            for i in 0..1000 {
                arr.push_back(black_box(&format!("value_{}", i)));
            }
            black_box(arr);
        });
    });

    group.bench_function("simply_list_vs_doubly_list_insert_1000", |b| {
        b.iter(|| {
            // Односвязный список
            let mut simply_list = SimplyList::new();
            for i in 0..1000 {
                simply_list.add_back(black_box(&format!("value_{}", i))).unwrap();
            }
            black_box(simply_list);

            // Двусвязный список
            let mut doubly_list = DoublyList::new();
            for i in 0..1000 {
                doubly_list.add_back(black_box(&format!("value_{}", i))).unwrap();
            }
            black_box(doubly_list);
        });
    });

    group.bench_function("stack_vs_queue_operations_1000", |b| {
        b.iter(|| {
            // Стек
            let mut stack = Stack::new();
            for i in 0..1000 {
                stack.push(black_box(&format!("value_{}", i)));
            }
            for _ in 0..1000 {
                black_box(stack.pop().unwrap());
            }

            // Очередь
            let mut queue = Queue::new();
            for i in 0..1000 {
                queue.enqueue(black_box(&format!("value_{}", i)));
            }
            for _ in 0..1000 {
                black_box(queue.dequeue().unwrap());
            }
        });
    });

    group.finish();
}

// Бенчмарки для больших объемов данных
fn bench_large_scale(c: &mut Criterion) {
    let mut group = c.benchmark_group("Large Scale");
    group.sample_size(10); // Уменьшаем количество samples для длительных тестов

    group.bench_function("array_10000_elements", |b| {
        b.iter(|| {
            let mut arr = DyArr::new(10000);
            for i in 0..10000 {
                arr.push_back(black_box(&format!("value_{}", i)));
            }
            black_box(arr);
        });
    });

    group.bench_function("hash_table_10000_elements", |b| {
        b.iter(|| {
            let mut table = OpenAddressingHashTable::new(20000);
            for i in 0..10000 {
                table.insert(
                    black_box(&format!("key_{}", i)),
                    black_box(&format!("value_{}", i))
                ).unwrap();
            }
            black_box(table);
        });
    });

    group.finish();
}

criterion_group!(
    benches,
    bench_array_operations,
    bench_simply_list_operations,
    bench_doubly_list_operations,
    bench_stack_operations,
    bench_queue_operations,
    bench_hash_table_operations,
    bench_comparison,
    bench_large_scale
);
criterion_main!(benches);