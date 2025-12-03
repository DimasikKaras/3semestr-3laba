use crate::storage::Storage;
use crate::structures::{DyArr, SimplyList, DoublyList, Stack, Queue, OpenAddressingHashTable, ChainHashTable};

pub fn process_commands(storage: &mut Storage, query: &str) -> Result<(), String> {
    // Разделяем команды по точке с запятой
    let commands: Vec<&str> = query.split(';').map(|cmd| cmd.trim()).collect();

    for command in commands {
        if command.is_empty() {
            continue;
        }

        let parts: Vec<&str> = command.split_whitespace().collect();
        if parts.is_empty() {
            continue;
        }

        match parts[0] {
            // Создание структур
            "MCREATE" => {
                if parts.len() != 2 {
                    return Err("MCREATE: неверное количество аргументов".to_string());
                }
                if storage.array_exists(parts[1]) {
                    return Err(format!("Массив с именем '{}' уже существует", parts[1]));
                }
                storage.arrays.insert(parts[1].to_string(), DyArr::new(10));
                println!("Массив '{}' создан", parts[1]);
            }
            "FCREATE" => {
                if parts.len() != 2 {
                    return Err("FCREATE: неверное количество аргументов".to_string());
                }
                if storage.simply_list_exists(parts[1]) {
                    return Err(format!("Односвязный список с именем '{}' уже существует", parts[1]));
                }
                storage.simply_lists.insert(parts[1].to_string(), SimplyList::new());
                println!("Односвязный список '{}' создан", parts[1]);
            }
            "DCREATE" => {
                if parts.len() != 2 {
                    return Err("DCREATE: неверное количество аргументов".to_string());
                }
                if storage.doubly_list_exists(parts[1]) {
                    return Err(format!("Двусвязный список с именем '{}' уже существует", parts[1]));
                }
                storage.doubly_lists.insert(parts[1].to_string(), DoublyList::new());
                println!("Двусвязный список '{}' создан", parts[1]);
            }
            "SCREATE" => {
                if parts.len() != 2 {
                    return Err("SCREATE: неверное количество аргументов".to_string());
                }
                if storage.stack_exists(parts[1]) {
                    return Err(format!("Стек с именем '{}' уже существует", parts[1]));
                }
                storage.stacks.insert(parts[1].to_string(), Stack::new());
                println!("Стек '{}' создан", parts[1]);
            }
            "QCREATE" => {
                if parts.len() != 2 {
                    return Err("QCREATE: неверное количество аргументов".to_string());
                }
                if storage.queue_exists(parts[1]) {
                    return Err(format!("Очередь с именем '{}' уже существует", parts[1]));
                }
                storage.queues.insert(parts[1].to_string(), Queue::new());
                println!("Очередь '{}' создан", parts[1]);
            }
            "TABLEOPENCREATE" => {
                if parts.len() != 2 {
                    return Err("TABLEOPENCREATE: неверное количество аргументов".to_string());
                }
                if storage.open_table_exists(parts[1]) {
                    return Err(format!("Хэш-таблица с именем '{}' уже существует", parts[1]));
                }
                storage.open_tables.insert(parts[1].to_string(), OpenAddressingHashTable::new(10));
                println!("Хэш-таблица (открытая адресация) '{}' создана", parts[1]);
            }
            "TABLECHAINCREATE" => {
                if parts.len() != 2 {
                    return Err("TABLECHAINCREATE: неверное количество аргументов".to_string());
                }
                if storage.chain_table_exists(parts[1]) {
                    return Err(format!("Хэш-таблица с именем '{}' уже существует", parts[1]));
                }
                storage.chain_tables.insert(parts[1].to_string(), ChainHashTable::new(10));
                println!("Хэш-таблица (метод цепочек) '{}' создана", parts[1]);
            }

            // Операции с массивом
            "MPUSH" => {
                if parts.len() != 3 {
                    return Err("MPUSH: неверное количество аргументов".to_string());
                }
                let arr = storage.arrays.get_mut(parts[1]).ok_or("Массив не найден")?;
                arr.push_back(parts[2]);
                println!("Значение '{}' добавлено в массив '{}'", parts[2], parts[1]);
            }
            "MPUSHINDEX" => {
                if parts.len() != 4 {
                    return Err("MPUSHINDEX: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let arr = storage.arrays.get_mut(parts[1]).ok_or("Массив не найден")?;
                arr.push_in_index(parts[3], index)?;
                println!("Значение '{}' добавлено в массив '{}' по индексу {}", parts[3], parts[1], index);
            }
            "MGET" => {
                if parts.len() != 3 {
                    return Err("MGET: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let arr = storage.arrays.get(parts[1]).ok_or("Массив не найден")?;
                let value = arr.get_element(index)?;
                println!("Значение по индексу {}: {}", index, value);
            }
            "MDELINDEX" => {
                if parts.len() != 3 {
                    return Err("MDELINDEX: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let arr = storage.arrays.get_mut(parts[1]).ok_or("Массив не найден")?;
                arr.remove_at_index(index)?;
                println!("Элемент с индексом {} удален из массива '{}'", index, parts[1]);
            }
            "MREPLACE" => {
                if parts.len() != 4 {
                    return Err("MREPLACE: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let arr = storage.arrays.get_mut(parts[1]).ok_or("Массив не найден")?;
                arr.replace_at_index(parts[3], index)?;
                println!("Элемент с индексом {} заменен на '{}' в массиве '{}'", index, parts[3], parts[1]);
            }
            "MLEN" => {
                if parts.len() != 2 {
                    return Err("MLEN: неверное количество аргументов".to_string());
                }
                let arr = storage.arrays.get(parts[1]).ok_or("Массив не найден")?;
                println!("Длина массива '{}': {}", parts[1], arr.get_length());
            }

            // Операции с односвязным списком
            "FPUSHHEAD" => {
                if parts.len() != 3 {
                    return Err("FPUSHHEAD: неверное количество аргументов".to_string());
                }
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_head(parts[2])?;
                println!("Значение '{}' добавлено в начало списка '{}'", parts[2], parts[1]);
            }
            "FPUSHTAIL" => {
                if parts.len() != 3 {
                    return Err("FPUSHTAIL: неверное количество аргументов".to_string());
                }
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_back(parts[2])?;
                println!("Значение '{}' добавлено в конец списка '{}'", parts[2], parts[1]);
            }
            "FPUSHAFTER" => {
                if parts.len() != 4 {
                    return Err("FPUSHAFTER: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_after_index(index, parts[3])?;
                println!("Значение '{}' добавлено после индекса {} в список '{}'", parts[3], index, parts[1]);
            }
            "FPUSHBEFORE" => {
                if parts.len() != 4 {
                    return Err("FPUSHBEFORE: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_before_index(index, parts[3])?;
                println!("Значение '{}' добавлено до индекса {} в список '{}'", parts[3], index, parts[1]);
            }
            "FDELHEAD" => {
                if parts.len() != 2 {
                    return Err("FDELHEAD: неверное количество аргументов".to_string());
                }
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_head()?;
                println!("Элемент удален из начала списка '{}'", parts[1]);
            }
            "FDELTAIL" => {
                if parts.len() != 2 {
                    return Err("FDELTAIL: неверное количество аргументов".to_string());
                }
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_back()?;
                println!("Элемент удален из конца списка '{}'", parts[1]);
            }
            "FDELAFTER" => {
                if parts.len() != 3 {
                    return Err("FDELAFTER: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_after_index(index)?;
                println!("Элемент после индекса {} удален из списка '{}'", index, parts[1]);
            }
            "FDELBEFORE" => {
                if parts.len() != 3 {
                    return Err("FDELBEFORE: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_before_index(index)?;
                println!("Элемент до индекса {} удален из списка '{}'", index, parts[1]);
            }
            "FDELVALUE" => {
                if parts.len() != 3 {
                    return Err("FDELVALUE: неверное количество аргументов".to_string());
                }
                let list = storage.simply_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_value(parts[2])?;
                println!("Элемент '{}' удален из списка '{}'", parts[2], parts[1]);
            }
            "FSEARCH" => {
                if parts.len() != 3 {
                    return Err("FSEARCH: неверное количество аргументов".to_string());
                }
                let list = storage.simply_lists.get(parts[1]).ok_or("Список не найден")?;
                let found = list.search_value(parts[2])?;
                println!("Элемент '{}' {}найден в списке '{}'", parts[2], if found { "" } else { "не " }, parts[1]);
            }

            // Операции с двусвязным списком (аналогично, но для DoublyList)
            "DPUSHHEAD" => {
                if parts.len() != 3 {
                    return Err("DPUSHHEAD: неверное количество аргументов".to_string());
                }
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_head(parts[2])?;
                println!("Значение '{}' добавлено в начало списка '{}'", parts[2], parts[1]);
            }
            "DPUSHTAIL" => {
                if parts.len() != 3 {
                    return Err("DPUSHTAIL: неверное количество аргументов".to_string());
                }
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_back(parts[2])?;
                println!("Значение '{}' добавлено в конец списка '{}'", parts[2], parts[1]);
            }
            "DPUSHAFTER" => {
                if parts.len() != 4 {
                    return Err("DPUSHAFTER: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_after_index(index, parts[3])?;
                println!("Значение '{}' добавлено после индекса {} в список '{}'", parts[3], index, parts[1]);
            }
            "DPUSHBEFORE" => {
                if parts.len() != 4 {
                    return Err("DPUSHBEFORE: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.add_before_index(index, parts[3])?;
                println!("Значение '{}' добавлено до индекса {} в список '{}'", parts[3], index, parts[1]);
            }
            "DDELHEAD" => {
                if parts.len() != 2 {
                    return Err("DDELHEAD: неверное количество аргументов".to_string());
                }
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_head()?;
                println!("Элемент удален из начала списка '{}'", parts[1]);
            }
            "DDELTAIL" => {
                if parts.len() != 2 {
                    return Err("DDELTAIL: неверное количество аргументов".to_string());
                }
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_back()?;
                println!("Элемент удален из конца списка '{}'", parts[1]);
            }
            "DDELAFTER" => {
                if parts.len() != 3 {
                    return Err("DDELAFTER: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_after_index(index)?;
                println!("Элемент после индекса {} удален из списка '{}'", index, parts[1]);
            }
            "DDELBEFORE" => {
                if parts.len() != 3 {
                    return Err("DDELBEFORE: неверное количество аргументов".to_string());
                }
                let index: usize = parts[2].parse().map_err(|_| "Неверный индекс")?;
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_before_index(index)?;
                println!("Элемент до индекса {} удален из списка '{}'", index, parts[1]);
            }
            "DDELVALUE" => {
                if parts.len() != 3 {
                    return Err("DDELVALUE: неверное количество аргументов".to_string());
                }
                let list = storage.doubly_lists.get_mut(parts[1]).ok_or("Список не найден")?;
                list.dell_value(parts[2])?;
                println!("Элемент '{}' удален из списка '{}'", parts[2], parts[1]);
            }
            "DSEARCH" => {
                if parts.len() != 3 {
                    return Err("DSEARCH: неверное количество аргументов".to_string());
                }
                let list = storage.doubly_lists.get(parts[1]).ok_or("Список не найден")?;
                let found = list.search_value(parts[2])?;
                println!("Элемент '{}' {}найден в списке '{}'", parts[2], if found { "" } else { "не " }, parts[1]);
            }

            // Операции со стеком
            "SPUSH" => {
                if parts.len() != 3 {
                    return Err("SPUSH: неверное количество аргументов".to_string());
                }
                let stack = storage.stacks.get_mut(parts[1]).ok_or("Стек не найден")?;
                stack.push(parts[2]);
                println!("Значение '{}' добавлено в стек '{}'", parts[2], parts[1]);
            }
            "SPOP" => {
                if parts.len() != 2 {
                    return Err("SPOP: неверное количество аргументов".to_string());
                }
                let stack = storage.stacks.get_mut(parts[1]).ok_or("Стек не найден")?;
                let value = stack.pop()?;
                println!("Значение '{}' извлечено из стека '{}'", value, parts[1]);
            }

            // Операции с очередью
            "QPUSH" => {
                if parts.len() != 3 {
                    return Err("QPUSH: неверное количество аргументов".to_string());
                }
                let queue = storage.queues.get_mut(parts[1]).ok_or("Очередь не найдена")?;
                queue.enqueue(parts[2]);
                println!("Значение '{}' добавлено в очередь '{}'", parts[2], parts[1]);
            }
            "QPOP" => {
                if parts.len() != 2 {
                    return Err("QPOP: неверное количество аргументов".to_string());
                }
                let queue = storage.queues.get_mut(parts[1]).ok_or("Очередь не найдена")?;
                let value = queue.dequeue()?;
                println!("Значение '{}' извлечено из очереди '{}'", value, parts[1]);
            }

            // Хэш таблица (открытая адресация)
            "TABLEOPENADD" => {
                if parts.len() != 4 {
                    return Err("TABLEOPENADD: неверное количество аргументов".to_string());
                }
                let table = storage.open_tables.get_mut(parts[1]).ok_or("Таблица не найдена")?;
                table.insert(parts[2], parts[3])?;
                println!("Элемент ('{}': '{}') добавлен в таблицу '{}'", parts[2], parts[3], parts[1]);
            }
            "TABLEOPENDEL" => {
                if parts.len() != 3 {
                    return Err("TABLEOPENDEL: неверное количество аргументов".to_string());
                }
                let table = storage.open_tables.get_mut(parts[1]).ok_or("Таблица не найдена")?;
                table.delete(parts[2])?;
                println!("Элемент с ключом '{}' удален из таблицы '{}'", parts[2], parts[1]);
            }
            "TABLEOPENGET" => {
                if parts.len() != 3 {
                    return Err("TABLEOPENGET: неверное количество аргументов".to_string());
                }
                let table = storage.open_tables.get(parts[1]).ok_or("Таблица не найдена")?;
                match table.search(parts[2])? {
                    Some((key, value)) => println!("Найден элемент: ключ '{}', значение '{}'", key, value),
                    None => println!("Элемент с ключом '{}' не найден", parts[2]),
                }
            }

            // Хэш таблица (метод цепочек)
            "TABLECHAINADD" => {
                if parts.len() != 4 {
                    return Err("TABLECHAINADD: неверное количество аргументов".to_string());
                }
                let table = storage.chain_tables.get_mut(parts[1]).ok_or("Таблица не найдена")?;
                table.insert(parts[2], parts[3])?;
                println!("Элемент ('{}': '{}') добавлен в таблицу '{}'", parts[2], parts[3], parts[1]);
            }
            "TABLECHAINDEL" => {
                if parts.len() != 3 {
                    return Err("TABLECHAINDEL: неверное количество аргументов".to_string());
                }
                let table = storage.chain_tables.get_mut(parts[1]).ok_or("Таблица не найдена")?;
                table.delete(parts[2])?;
                println!("Элемент с ключом '{}' удален из таблицы '{}'", parts[2], parts[1]);
            }
            "TABLECHAINGET" => {
                if parts.len() != 3 {
                    return Err("TABLECHAINGET: неверное количество аргументов".to_string());
                }
                let table = storage.chain_tables.get(parts[1]).ok_or("Таблица не найдена")?;
                match table.search(parts[2])? {
                    Some((key, value)) => println!("Найден элемент: ключ '{}', значение '{}'", key, value),
                    None => println!("Элемент с ключом '{}' не найден", parts[2]),
                }
            }

            // Общие команды
            "PRINT" => {
                if parts.len() != 2 {
                    return Err("PRINT: неверное количество аргументов".to_string());
                }
                println!("=== Структура '{}' ===", parts[1]);

                if let Some(arr) = storage.arrays.get(parts[1]) {
                    println!("Тип: Массив");
                    arr.print_dy_arr();
                } else if let Some(list) = storage.simply_lists.get(parts[1]) {
                    println!("Тип: Односвязный список");
                    list.print();
                } else if let Some(list) = storage.doubly_lists.get(parts[1]) {
                    println!("Тип: Двусвязный список");
                    list.print();
                } else if let Some(stack) = storage.stacks.get(parts[1]) {
                    println!("Тип: Стек");
                    stack.print();
                } else if let Some(queue) = storage.queues.get(parts[1]) {
                    println!("Тип: Очередь");
                    queue.print();
                } else if let Some(table) = storage.open_tables.get(parts[1]) {
                    println!("Тип: Хэш-таблица (открытая адресация)");
                    table.print();
                } else if let Some(table) = storage.chain_tables.get(parts[1]) {
                    println!("Тип: Хэш-таблица (метод цепочек)");
                    table.print();
                } else {
                    return Err(format!("Структура '{}' не найдена", parts[1]));
                }
            }

            _ => {
                return Err(format!("Неизвестная команда: {}", parts[0]));
            }
        }
    }

    Ok(())
}