use std::fmt;
use serde::{Deserialize, Serialize};

#[derive(Serialize, Deserialize, Clone)]
pub struct DoublyListNode {
    data: String,
    next: Option<Box<DoublyListNode>>,
    #[serde(skip)]
    prev: Option<*mut DoublyListNode>,
}

impl DoublyListNode {
    pub fn new(value: &str) -> Self {
        DoublyListNode {
            data: value.to_string(),
            next: None,
            prev: None,
        }
    }

    pub fn get_data(&self) -> &str {
        &self.data
    }

    pub fn get_next(&self) -> Option<&DoublyListNode> {
        self.next.as_deref()
    }

    pub fn get_prev(&self) -> Option<&DoublyListNode> {
        unsafe { self.prev.map(|ptr| &*ptr) }
    }

    pub fn set_next(&mut self, node: Option<Box<DoublyListNode>>) {
        self.next = node;
    }

    pub fn set_prev(&mut self, node: Option<*mut DoublyListNode>) {
        self.prev = node;
    }
}

#[derive(Serialize, Deserialize, Clone)]
pub struct DoublyList {
    head: Option<Box<DoublyListNode>>,
    // Убираем tail указатель для сериализации
    #[serde(skip)]
    tail: Option<*mut DoublyListNode>,
}

impl DoublyList {
    pub fn new() -> Self {
        DoublyList {
            head: None,
            tail: None,
        }
    }

    pub fn get_head(&self) -> Option<&DoublyListNode> {
        self.head.as_deref()
    }

    pub fn add_head(&mut self, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        let mut new_node = Box::new(DoublyListNode::new(value));
        let new_node_ptr: *mut DoublyListNode = &mut *new_node;

        match self.head.take() {
            Some(mut old_head) => {
                old_head.set_prev(Some(new_node_ptr));
                new_node.set_next(Some(old_head));
                new_node.set_prev(None);
                self.head = Some(new_node);
            }
            None => {
                new_node.set_prev(None);
                self.head = Some(new_node);
                self.tail = Some(new_node_ptr);
            }
        }
        Ok(())
    }

    pub fn add_back(&mut self, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        let mut new_node = Box::new(DoublyListNode::new(value));
        let new_node_ptr: *mut DoublyListNode = &mut *new_node;

        match self.tail {
            Some(old_tail_ptr) => unsafe {
                // Сохраняем next нового узла перед перемещением
                let next = new_node.next.take();
                (*old_tail_ptr).set_next(Some(new_node));
                if let Some(ref mut node) = (*old_tail_ptr).next {
                    node.set_prev(Some(old_tail_ptr));
                    node.set_next(next);
                }
                self.tail = Some(new_node_ptr);
            },
            None => {
                new_node.set_prev(None);
                self.head = Some(new_node);
                self.tail = Some(new_node_ptr);
            }
        }
        Ok(())
    }

    pub fn add_after_index(&mut self, index: usize, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        if self.head.is_none() {
            if index == 0 {
                return self.add_head(value);
            } else {
                return Err("Список пуст, индекс недоступен");
            }
        }

        let mut current = &mut self.head;
        for _ in 0..index {
            match current {
                Some(node) => {
                    current = &mut node.next;
                }
                None => return Err("Индекс превышает размер списка"),
            }
        }

        if let Some(node) = current {
            let mut new_node = Box::new(DoublyListNode::new(value));
            let new_node_ptr: *mut DoublyListNode = &mut *new_node;

            new_node.set_next(node.next.take());
            new_node.set_prev(Some(&mut **node as *mut DoublyListNode));

            if let Some(ref mut next_node) = new_node.next {
                next_node.set_prev(Some(new_node_ptr));
            } else {
                self.tail = Some(new_node_ptr);
            }

            node.set_next(Some(new_node));
            Ok(())
        } else {
            Err("Индекс превышает размер списка")
        }
    }

    pub fn add_before_index(&mut self, index: usize, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        if index == 0 {
            return self.add_head(value);
        }

        self.add_after_index(index - 1, value)
    }

    pub fn dell_head(&mut self) -> Result<(), &'static str> {
        match self.head.take() {
            Some(mut node) => {
                if let Some(mut next_node) = node.next.take() {
                    next_node.set_prev(None);
                    self.head = Some(next_node);
                } else {
                    self.head = None;
                    self.tail = None;
                }
                Ok(())
            }
            None => Err("Список пустой"),
        }
    }

    pub fn dell_back(&mut self) -> Result<(), &'static str> {
        match self.tail.take() {
            Some(tail_ptr) => unsafe {
                if let Some(prev_ptr) = (*tail_ptr).prev {
                    (*prev_ptr).set_next(None);
                    self.tail = Some(prev_ptr);
                } else {
                    self.head = None;
                    self.tail = None;
                }
                Ok(())
            },
            None => Err("Список пустой"),
        }
    }

    pub fn dell_after_index(&mut self, index: usize) -> Result<(), &'static str> {
        if self.head.is_none() {
            return Err("Список пустой");
        }

        let mut current = &mut self.head;
        for _ in 0..index {
            match current {
                Some(node) => {
                    current = &mut node.next;
                }
                None => return Err("Индекс превышает размер списка"),
            }
        }

        if let Some(node) = current {
            if let Some(mut next_node) = node.next.take() {
                if let Some(mut next_next_node) = next_node.next.take() {
                    next_next_node.set_prev(Some(&mut **node as *mut DoublyListNode));
                    node.set_next(Some(next_next_node));
                } else {
                    node.set_next(None);
                    self.tail = Some(&mut **node as *mut DoublyListNode);
                }
                Ok(())
            } else {
                Err("Нет элемента после индекса")
            }
        } else {
            Err("Индекс превышает размер списка")
        }
    }

    pub fn dell_before_index(&mut self, index: usize) -> Result<(), &'static str> {
        if index == 0 {
            return Err("Нельзя удалить перед индексом 0");
        }

        if index == 1 {
            return self.dell_head();
        }

        self.dell_after_index(index - 2)
    }

    pub fn dell_value(&mut self, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        if self.head.is_none() {
            return Ok(());
        }

        // Проверяем голову
        if self.head.as_ref().unwrap().data == value {
            return self.dell_head();
        }

        // Ищем узел с нужным значением
        let mut current = &mut self.head;
        while current.as_ref().is_some() && current.as_ref().unwrap().data != value {
            current = &mut current.as_mut().unwrap().next;
        }

        if current.is_none() {
            return Ok(());
        }

        if let Some(mut node_to_delete) = current.take() {
            if let Some(prev_ptr) = node_to_delete.prev {
                unsafe {
                    if let Some(next_node) = node_to_delete.next.take() {
                        (*prev_ptr).set_next(Some(next_node));
                        if let Some(ref mut next_node_ref) = (*prev_ptr).next {
                            next_node_ref.set_prev(Some(prev_ptr));
                        }
                    } else {
                        (*prev_ptr).set_next(None);
                        self.tail = Some(prev_ptr);
                    }
                }
            }
            Ok(())
        } else {
            Ok(())
        }
    }

    pub fn search_value(&self, value: &str) -> Result<bool, &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        let mut current = &self.head;
        while let Some(node) = current {
            if node.data == value {
                return Ok(true);
            }
            current = &node.next;
        }
        Ok(false)
    }

    pub fn print(&self) {
        if self.head.is_none() {
            println!("(Список пустой)");
            return;
        }

        let mut current = &self.head;
        while let Some(node) = current {
            print!("{}", node.data);
            if node.next.is_some() {
                print!(" <-> ");
            }
            current = &node.next;
        }
        println!();
    }

    pub fn print_reverse(&self) {
        match self.tail {
            Some(tail_ptr) => {
                let mut current = Some(unsafe { &*tail_ptr });
                while let Some(node) = current {
                    print!("{}", node.data);
                    if node.prev.is_some() {
                        print!(" <-> ");
                    }
                    current = node.get_prev();
                }
                println!();
            }
            None => println!("(Список пустой)"),
        }
    }

    // Метод для восстановления указателей после десериализации
    pub fn rebuild_pointers(&mut self) {
        if self.head.is_none() {
            self.tail = None;
            return;
        }

        // Восстанавливаем prev указатели и tail
        let mut current: Option<*mut DoublyListNode> = None;
        let mut node_ptr = &mut self.head as *mut Option<Box<DoublyListNode>>;

        unsafe {
            while let Some(node) = &mut *node_ptr {
                node.prev = current;
                current = Some(&mut **node as *mut DoublyListNode);

                if node.next.is_none() {
                    self.tail = current;
                }

                node_ptr = &mut node.next as *mut Option<Box<DoublyListNode>>;
            }
        }
    }
}

impl Drop for DoublyList {
    fn drop(&mut self) {
        while self.dell_head().is_ok() {}
    }
}

impl fmt::Display for DoublyList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.head.is_none() {
            return write!(f, "(Список пустой)");
        }

        let mut current = &self.head;
        let mut first = true;

        while let Some(node) = current {
            if !first {
                write!(f, " <-> ")?;
            }
            write!(f, "{}", node.data)?;
            current = &node.next;
            first = false;
        }

        Ok(())
    }
}