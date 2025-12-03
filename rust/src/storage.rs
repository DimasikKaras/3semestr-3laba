use serde::{Deserialize, Serialize};
use std::collections::HashMap;
use std::fs;
use std::io;
use bincode;

use crate::structures::{
    DyArr, SimplyList, DoublyList, Stack, Queue,
    OpenAddressingHashTable, ChainHashTable
};

#[derive(Serialize, Deserialize, Clone)]
pub struct Storage {
    pub arrays: HashMap<String, DyArr>,
    pub simply_lists: HashMap<String, SimplyList>,
    pub doubly_lists: HashMap<String, DoublyList>,
    pub stacks: HashMap<String, Stack>,
    pub queues: HashMap<String, Queue>,
    pub open_tables: HashMap<String, OpenAddressingHashTable>,
    pub chain_tables: HashMap<String, ChainHashTable>,
}

impl Storage {
    pub fn new() -> Self {
        Storage {
            arrays: HashMap::new(),
            simply_lists: HashMap::new(),
            doubly_lists: HashMap::new(),
            stacks: HashMap::new(),
            queues: HashMap::new(),
            open_tables: HashMap::new(),
            chain_tables: HashMap::new(),
        }
    }

    // Сохранение в простом текстовом формате
    pub fn save_to_text(&self, filename: &str) -> Result<(), io::Error> {
        let mut content = String::new();

        // Массивы
        for (name, arr) in &self.arrays {
            content.push_str(&format!("ARRAY {}: [", name));
            for (i, item) in arr.data.iter().enumerate() {
                if i > 0 {
                    content.push_str(", ");
                }
                content.push_str(item);
            }
            content.push_str("]\n");
        }

        // Односвязные списки
        for (name, list) in &self.simply_lists {
            content.push_str(&format!("SIMPLY {}: ", name));
            // Используем метод Display для форматирования
            content.push_str(&list.to_string());
            content.push('\n');
        }

        // Двусвязные списки
        for (name, list) in &self.doubly_lists {
            content.push_str(&format!("DOUBLY {}: ", name));
            // Используем метод Display для форматирования
            content.push_str(&list.to_string());
            content.push('\n');
        }

        // Стеки
        for (name, stack) in &self.stacks {
            content.push_str(&format!("STACK {}: ", name));
            // Используем метод Display для форматирования
            content.push_str(&stack.to_string());
            content.push('\n');
        }

        // Очереди
        for (name, queue) in &self.queues {
            content.push_str(&format!("QUEUE {}: ", name));
            // Используем метод Display для форматирования
            content.push_str(&queue.to_string());
            content.push('\n');
        }

        // Хэш-таблицы с открытой адресацией
        for (name, table) in &self.open_tables {
            content.push_str(&format!("OPEN_TABLE {}:\n", name));
            for (key, value) in table.get_all_elements() {
                content.push_str(&format!("  {} -> {}\n", key, value));
            }
        }

        // Хэш-таблицы с цепочками
        for (name, table) in &self.chain_tables {
            content.push_str(&format!("CHAIN_TABLE {}:\n", name));
            for (key, value) in table.get_all_elements() {
                content.push_str(&format!("  {} -> {}\n", key, value));
            }
        }

        fs::write(filename, content)
    }

    // Загрузка из простого текстового формата
    pub fn load_from_text(filename: &str) -> Result<Self, io::Error> {
        let content = match fs::read_to_string(filename) {
            Ok(content) => content,
            Err(e) if e.kind() == io::ErrorKind::NotFound => {
                // Файл не существует, создаем новое хранилище
                return Ok(Storage::new());
            }
            Err(e) => return Err(e),
        };

        let mut storage = Storage::new();

        for line in content.lines() {
            if line.is_empty() {
                continue;
            }

            if let Some(rest) = line.strip_prefix("SIMPLY ") {
                if let Some(colon_pos) = rest.find(':') {
                    let name = rest[..colon_pos].trim().to_string();
                    let values_str = rest[colon_pos + 1..].trim();

                    let mut list = SimplyList::new();
                    for value in values_str.split("->").map(|s| s.trim()).filter(|s| !s.is_empty()) {
                        if let Err(_) = list.add_back(value) {
                            // Игнорируем ошибки добавления
                        }
                    }
                    storage.simply_lists.insert(name, list);
                }
            } else if let Some(rest) = line.strip_prefix("ARRAY ") {
                if let Some(colon_pos) = rest.find(':') {
                    let name = rest[..colon_pos].trim().to_string();
                    let values_str = rest[colon_pos + 1..].trim();

                    let mut arr = DyArr::new(10);
                    // Парсим массив вида [val1, val2, val3]
                    if values_str.starts_with('[') && values_str.ends_with(']') {
                        let values = &values_str[1..values_str.len() - 1];
                        for value in values.split(',').map(|s| s.trim()).filter(|s| !s.is_empty()) {
                            arr.push_back(value);
                        }
                    }
                    storage.arrays.insert(name, arr);
                }
            } else if let Some(rest) = line.strip_prefix("DOUBLY ") {
                if let Some(colon_pos) = rest.find(':') {
                    let name = rest[..colon_pos].trim().to_string();
                    let values_str = rest[colon_pos + 1..].trim();

                    let mut list = DoublyList::new();
                    for value in values_str.split("<->").map(|s| s.trim()).filter(|s| !s.is_empty()) {
                        if let Err(_) = list.add_back(value) {
                            // Игнорируем ошибки добавления
                        }
                    }
                    storage.doubly_lists.insert(name, list);
                }
            } else if let Some(rest) = line.strip_prefix("STACK ") {
                if let Some(colon_pos) = rest.find(':') {
                    let name = rest[..colon_pos].trim().to_string();
                    let values_str = rest[colon_pos + 1..].trim();

                    let mut stack = Stack::new();
                    // Стек парсим в обратном порядке (последний добавленный - первый)
                    let values: Vec<&str> = values_str.split("->").map(|s| s.trim()).filter(|s| !s.is_empty()).collect();
                    for &value in values.iter().rev() {
                        stack.push(value);
                    }
                    storage.stacks.insert(name, stack);
                }
            } else if let Some(rest) = line.strip_prefix("QUEUE ") {
                if let Some(colon_pos) = rest.find(':') {
                    let name = rest[..colon_pos].trim().to_string();
                    let values_str = rest[colon_pos + 1..].trim();

                    let mut queue = Queue::new();
                    for value in values_str.split("->").map(|s| s.trim()).filter(|s| !s.is_empty()) {
                        queue.enqueue(value);
                    }
                    storage.queues.insert(name, queue);
                }
            }
            // Для хэш-таблиц нужна более сложная логика парсинга
        }

        // Восстанавливаем указатели
        storage.rebuild_pointers();
        Ok(storage)
    }

    // Сохранение в бинарном формате
    pub fn save_to_binary(&self, filename: &str) -> Result<(), io::Error> {
        let encoded = bincode::serialize(self)
            .map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;
        fs::write(filename, &encoded)?;
        println!("Сохранено в бинарный файл: {} ({} байт)", filename, encoded.len());
        Ok(())
    }

    // Загрузка из бинарного формата
    pub fn load_from_binary(filename: &str) -> Result<Self, io::Error> {
        let data = fs::read(filename)?;
        let mut storage: Storage = bincode::deserialize(&data)
            .map_err(|e| io::Error::new(io::ErrorKind::InvalidData, e))?;

        println!("Загружено из бинарного файла: {} ({} байт)", filename, data.len());
        storage.rebuild_pointers();
        Ok(storage)
    }

    // Восстановление указателей после десериализации
    fn rebuild_pointers(&mut self) {
        for (_, list) in self.doubly_lists.iter_mut() {
            list.rebuild_pointers();
        }

        for (_, queue) in self.queues.iter_mut() {
            queue.rebuild_pointers();
        }
    }

    // Методы для проверки существования структур
    pub fn array_exists(&self, name: &str) -> bool {
        self.arrays.contains_key(name)
    }

    pub fn simply_list_exists(&self, name: &str) -> bool {
        self.simply_lists.contains_key(name)
    }

    pub fn doubly_list_exists(&self, name: &str) -> bool {
        self.doubly_lists.contains_key(name)
    }

    pub fn stack_exists(&self, name: &str) -> bool {
        self.stacks.contains_key(name)
    }

    pub fn queue_exists(&self, name: &str) -> bool {
        self.queues.contains_key(name)
    }

    pub fn open_table_exists(&self, name: &str) -> bool {
        self.open_tables.contains_key(name)
    }

    pub fn chain_table_exists(&self, name: &str) -> bool {
        self.chain_tables.contains_key(name)
    }
}