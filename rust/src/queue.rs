use std::fmt;
use serde::{Deserialize, Serialize};

#[derive(Clone, Serialize, Deserialize)]
pub struct QueueNode {
    data: String,
    next: Option<Box<QueueNode>>,
}

impl QueueNode {
    pub fn new(value: &str) -> Self {
        QueueNode {
            data: value.to_string(),
            next: None,
        }
    }

    pub fn get_data(&self) -> &str {
        &self.data
    }

    pub fn set_next(&mut self, node: Option<Box<QueueNode>>) {
        self.next = node;
    }
}

#[derive(Clone, Serialize, Deserialize)]
pub struct Queue {
    first: Option<Box<QueueNode>>,
    // Убираем last указатель для сериализации
    #[serde(skip)]
    last: Option<*mut QueueNode>,
}

impl Queue {
    pub fn new() -> Self {
        Queue {
            first: None,
            last: None,
        }
    }

    pub fn get_first(&self) -> Option<&QueueNode> {
        self.first.as_deref()
    }

    pub fn enqueue(&mut self, value: &str) {
        let mut new_node = Box::new(QueueNode::new(value));
        let new_node_ptr: *mut QueueNode = &mut *new_node;

        match self.last.take() {
            Some(last_ptr) => {
                unsafe {
                    (*last_ptr).set_next(Some(new_node));
                }
                self.last = Some(new_node_ptr);
            }
            None => {
                // Если очередь пуста
                self.first = Some(new_node);
                self.last = Some(new_node_ptr);
            }
        }
    }

    pub fn dequeue(&mut self) -> Result<String, &'static str> {
        match self.first.take() {
            Some(node) => {
                self.first = node.next;

                // Если очередь стала пустой, обновляем last
                if self.first.is_none() {
                    self.last = None;
                }

                Ok(node.data)
            }
            None => Err("Очередь пустая"),
        }
    }

    pub fn print(&self) {
        if self.first.is_none() {
            println!("(Пустая очередь)");
            return;
        }

        let mut current = &self.first;
        while let Some(node) = current {
            print!("{} ", node.get_data());
            current = &node.next;
        }
        println!();
    }

    // Метод для восстановления указателей после десериализации
    pub fn rebuild_pointers(&mut self) {
        if self.first.is_none() {
            self.last = None;
            return;
        }

        // Находим последний элемент
        let mut current = &mut self.first;
        while current.as_ref().unwrap().next.is_some() {
            current = &mut current.as_mut().unwrap().next;
        }

        // Устанавливаем last указатель
        if let Some(last_node) = current {
            self.last = Some(&mut **last_node as *mut QueueNode);
        }
    }
}

impl Drop for Queue {
    fn drop(&mut self) {
        while self.dequeue().is_ok() {}
    }
}

impl fmt::Display for Queue {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.first.is_none() {
            return write!(f, "(Пустая очередь)");
        }

        let mut current = &self.first;
        let mut first = true;

        while let Some(node) = current {
            if !first {
                write!(f, " -> ")?;
            }
            write!(f, "{}", node.get_data())?;
            current = &node.next;
            first = false;
        }

        Ok(())
    }
}