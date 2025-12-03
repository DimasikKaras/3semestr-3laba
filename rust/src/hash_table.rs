use std::fmt;
use std::collections::LinkedList;
use serde::{Deserialize, Serialize};

const BASE: u64 = 2166136261;
const PRIME: u64 = 16777619;
const PRIMES: [usize; 13] = [5, 7, 11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853];

// ==================== ХЭШ-ТАБЛИЦА С ОТКРЫТОЙ АДРЕСАЦИЕЙ ====================

#[derive(Clone, Serialize, Deserialize)]
enum OpenAddressingSlot {
    Empty,
    Deleted,
    Occupied(String, String),
}
#[derive(Clone, Serialize, Deserialize)]
pub struct OpenAddressingHashTable {
    table: Vec<OpenAddressingSlot>,
    capacity: usize,
    size: usize,
}

impl OpenAddressingHashTable {
    pub fn new(capacity: usize) -> Self {
        if capacity == 0 {
            panic!("Вместимость должна быть больше 0");
        }

        let mut table = Vec::with_capacity(capacity);
        for _ in 0..capacity {
            table.push(OpenAddressingSlot::Empty);
        }

        OpenAddressingHashTable {
            table,
            capacity,
            size: 0,
        }
    }

    fn hash_function(&self, key: &str) -> usize {
        let mut hash = BASE;

        for byte in key.bytes() {
            hash ^= byte as u64;
            hash = hash.wrapping_mul(PRIME);
        }

        (hash % self.capacity as u64) as usize
    }

    pub fn insert(&mut self, key: &str, value: &str) -> Result<bool, &'static str> {
        if key.is_empty() || value.is_empty() {
            return Err("ключ и значение не могут быть пустыми");
        }

        // Проверяем необходимость рехэширования
        if self.size * 4 >= self.capacity * 3 { // 0.75 коэффициент
            if !self.rehash() {
                return Ok(false);
            }
        }

        let mut hash = self.hash_function(key);
        let mut first_deleted: Option<usize> = None;

        // Поиск места для вставки
        for i in 0..self.capacity {
            let index = (hash + i) % self.capacity;

            match &self.table[index] {
                OpenAddressingSlot::Occupied(k, _) if k == key => {
                    // Ключ уже существует - обновляем значение
                    self.table[index] = OpenAddressingSlot::Occupied(key.to_string(), value.to_string());
                    return Ok(true);
                }
                OpenAddressingSlot::Empty => {
                    // Нашли пустой слот
                    let insert_index = first_deleted.unwrap_or(index);
                    self.table[insert_index] = OpenAddressingSlot::Occupied(key.to_string(), value.to_string());
                    self.size += 1;
                    return Ok(true);
                }
                OpenAddressingSlot::Deleted if first_deleted.is_none() => {
                    // Запоминаем первый удаленный слот
                    first_deleted = Some(index);
                }
                _ => {}
            }
        }

        // Используем первый удаленный слот, если нашли
        if let Some(index) = first_deleted {
            self.table[index] = OpenAddressingSlot::Occupied(key.to_string(), value.to_string());
            self.size += 1;
            return Ok(true);
        }

        Ok(false)
    }

    pub fn delete(&mut self, key: &str) -> Result<bool, &'static str> {
        if key.is_empty() {
            return Err("ключ не может быть пустым");
        }

        let hash = self.hash_function(key);

        for i in 0..self.capacity {
            let index = (hash + i) % self.capacity;

            match &self.table[index] {
                OpenAddressingSlot::Occupied(k, _) if k == key => {
                    self.table[index] = OpenAddressingSlot::Deleted;
                    self.size -= 1;
                    return Ok(true);
                }
                OpenAddressingSlot::Empty => break,
                _ => {}
            }
        }

        Ok(false)
    }

    pub fn search(&self, key: &str) -> Result<Option<(&str, &str)>, &'static str> {
        if key.is_empty() {
            return Err("ключ не может быть пустым");
        }

        let hash = self.hash_function(key);

        for i in 0..self.capacity {
            let index = (hash + i) % self.capacity;

            match &self.table[index] {
                OpenAddressingSlot::Occupied(k, v) if k == key => {
                    return Ok(Some((k, v)));
                }
                OpenAddressingSlot::Empty => break,
                _ => {}
            }
        }

        Ok(None)
    }

    fn rehash(&mut self) -> bool {
        // Сохраняем все элементы
        let elements: Vec<(String, String)> = self.table.iter()
            .filter_map(|slot| {
                if let OpenAddressingSlot::Occupied(k, v) = slot {
                    Some((k.clone(), v.clone()))
                } else {
                    None
                }
            })
            .collect();

        // Находим новый размер
        let new_capacity = PRIMES.iter()
            .find(|&&p| p > self.capacity)
            .copied()
            .unwrap_or(self.capacity * 2);

        // Создаем новую таблицу
        self.table = Vec::with_capacity(new_capacity);
        for _ in 0..new_capacity {
            self.table.push(OpenAddressingSlot::Empty);
        }
        self.capacity = new_capacity;
        self.size = 0;

        // Вставляем элементы обратно
        for (key, value) in elements {
            if self.insert(&key, &value).is_err() {
                return false;
            }
        }

        true
    }

    pub fn print(&self) {
        println!("Хэш функция с методом открытой адресации");
        println!("Размер: {}/{}", self.size, self.capacity);

        for (i, slot) in self.table.iter().enumerate() {
            match slot {
                OpenAddressingSlot::Empty => println!("[{}] [NULL]", i),
                OpenAddressingSlot::Deleted => println!("[{}] [DELETE]", i),
                OpenAddressingSlot::Occupied(k, v) => {
                    println!("[{}] <{}> -> <{}>", i, k, v);
                }
            }
        }
    }

    pub fn get_size(&self) -> usize {
        self.size
    }

    pub fn get_capacity(&self) -> usize {
        self.capacity
    }
}

// ==================== ХЭШ-ТАБЛИЦА С МЕТОДОМ ЦЕПОЧЕК ====================

type ChainNode = LinkedList<(String, String)>;
#[derive(Clone, Serialize, Deserialize)]
pub struct ChainHashTable {
    table: Vec<ChainNode>,
    capacity: usize,
    size: usize,
}

impl ChainHashTable {
    pub fn new(capacity: usize) -> Self {
        let mut table = Vec::with_capacity(capacity);
        for _ in 0..capacity {
            table.push(LinkedList::new());
        }

        ChainHashTable {
            table,
            capacity,
            size: 0,
        }
    }

    fn hash_function(&self, key: &str) -> usize {
        let mut hash = BASE;

        for byte in key.bytes() {
            hash ^= byte as u64;
            hash = hash.wrapping_mul(PRIME);
        }

        (hash % self.capacity as u64) as usize
    }

    pub fn insert(&mut self, key: &str, value: &str) -> Result<bool, &'static str> {
        if key.is_empty() || value.is_empty() {
            return Err("ключ и значение не могут быть пустыми");
        }

        // Проверяем необходимость рехэширования
        if self.size * 4 >= self.capacity * 3 {
            if !self.rehash() {
                return Ok(false);
            }
        }

        let index = self.hash_function(key);
        let bucket = &mut self.table[index];

        // Ищем ключ в цепочке
        for pair in bucket.iter_mut() {
            if pair.0 == key {
                pair.1 = value.to_string();
                return Ok(true);
            }
        }

        // Ключ не найден - добавляем новый
        bucket.push_back((key.to_string(), value.to_string()));
        self.size += 1;
        Ok(true)
    }

    pub fn delete(&mut self, key: &str) -> Result<bool, &'static str> {
        if key.is_empty() {
            return Err("ключ не может быть пустым");
        }

        let index = self.hash_function(key);
        let bucket = &mut self.table[index];

        let mut found_index = None;
        for (i, pair) in bucket.iter().enumerate() {
            if pair.0 == key {
                found_index = Some(i);
                break;
            }
        }

        if let Some(i) = found_index {
            // Удаляем элемент из цепочки
            let mut split_list = bucket.split_off(i);
            split_list.pop_front(); // Удаляем нужный элемент
            bucket.append(&mut split_list);
            self.size -= 1;
            Ok(true)
        } else {
            Ok(false)
        }
    }

    pub fn search(&self, key: &str) -> Result<Option<(&str, &str)>, &'static str> {
        if key.is_empty() {
            return Err("ключ не может быть пустым");
        }

        let index = self.hash_function(key);
        let bucket = &self.table[index];

        for pair in bucket {
            if pair.0 == key {
                return Ok(Some((&pair.0, &pair.1)));
            }
        }

        Ok(None)
    }

    fn rehash(&mut self) -> bool {
        // Сохраняем все элементы
        let elements: Vec<(String, String)> = self.table.iter()
            .flat_map(|bucket| bucket.iter().cloned())
            .collect();

        // Находим новый размер
        let new_capacity = PRIMES.iter()
            .find(|&&p| p > self.capacity)
            .copied()
            .unwrap_or(self.capacity * 2);

        // Создаем новую таблицу
        self.table = Vec::with_capacity(new_capacity);
        for _ in 0..new_capacity {
            self.table.push(LinkedList::new());
        }
        self.capacity = new_capacity;
        self.size = 0;

        // Вставляем элементы обратно
        for (key, value) in elements {
            if self.insert(&key, &value).is_err() {
                return false;
            }
        }

        true
    }

    pub fn print(&self) {
        println!("Хэш функция с методом цепочек");
        println!("Размер: {}/{}", self.size, self.capacity);

        for (i, bucket) in self.table.iter().enumerate() {
            print!("[{}] ", i);

            if bucket.is_empty() {
                println!("nullptr");
            } else {
                for (j, (key, value)) in bucket.iter().enumerate() {
                    if j > 0 {
                        print!(" -> ");
                    }
                    print!("({}, {})", key, value);
                }
                println!(" -> nullptr");
            }
        }
    }

    pub fn get_size(&self) -> usize {
        self.size
    }

    pub fn get_capacity(&self) -> usize {
        self.capacity
    }
}

impl fmt::Display for OpenAddressingHashTable {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "OpenAddressingHashTable({}/{})", self.size, self.capacity)
    }
}

impl fmt::Display for ChainHashTable {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "ChainHashTable({}/{})", self.size, self.capacity)
    }
}

impl OpenAddressingHashTable {
    pub fn get_all_elements(&self) -> Vec<(String, String)> {
        let mut elements = Vec::new();
        for slot in &self.table {
            if let OpenAddressingSlot::Occupied(k, v) = slot {
                elements.push((k.clone(), v.clone()));
            }
        }
        elements
    }
}

impl ChainHashTable {
    pub fn get_all_elements(&self) -> Vec<(String, String)> {
        let mut elements = Vec::new();
        for bucket in &self.table {
            for pair in bucket {
                elements.push((pair.0.clone(), pair.1.clone()));
            }
        }
        elements
    }
}