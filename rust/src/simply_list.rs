use std::fmt;
use serde::{Deserialize, Serialize};

#[derive(Clone, Serialize, Deserialize)]
pub struct SimplyListNode {
    data: String,
    next: Option<Box<SimplyListNode>>,
}

impl SimplyListNode {
    pub fn new(value: &str) -> Self {
        SimplyListNode {
            data: value.to_string(),
            next: None,
        }
    }

    pub fn get_data(&self) -> &str {
        &self.data
    }

    pub fn get_next(&self) -> Option<&SimplyListNode> {
        self.next.as_deref()
    }

    pub fn set_next(&mut self, node: Option<Box<SimplyListNode>>) {
        self.next = node;
    }
}
#[derive(Clone, Serialize, Deserialize)]
pub struct SimplyList {
    pub(crate) head: Option<Box<SimplyListNode>>,
}

impl SimplyList {
    pub fn new() -> Self {
        SimplyList { head: None }
    }

    pub fn get_head(&self) -> Option<&SimplyListNode> {
        self.head.as_deref()
    }

    pub fn add_head(&mut self, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        let mut new_node = Box::new(SimplyListNode::new(value));
        new_node.set_next(self.head.take());
        self.head = Some(new_node);
        Ok(())
    }

    pub fn add_back(&mut self, value: &str) -> Result<(), &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        if self.head.is_none() {
            return self.add_head(value);
        }

        let mut current = &mut self.head;
        while current.as_ref().unwrap().next.is_some() {
            current = &mut current.as_mut().unwrap().next;
        }

        current.as_mut().unwrap().set_next(Some(Box::new(SimplyListNode::new(value))));
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
            let mut new_node = Box::new(SimplyListNode::new(value));
            new_node.set_next(node.next.take());
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
            Some(node) => {
                self.head = node.next;
                Ok(())
            }
            None => Err("Список пустой"),
        }
    }

    pub fn dell_back(&mut self) -> Result<(), &'static str> {
        if self.head.is_none() {
            return Err("Список пустой");
        }

        if self.head.as_ref().unwrap().next.is_none() {
            return self.dell_head();
        }

        let mut current = &mut self.head;
        while current.as_ref().unwrap().next.as_ref().unwrap().next.is_some() {
            current = &mut current.as_mut().unwrap().next;
        }

        current.as_mut().unwrap().set_next(None);
        Ok(())
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
            if node.next.is_some() {
                let next_node = node.next.take().unwrap();
                node.set_next(next_node.next);
                Ok(())
            } else {
                Err("Индекс превышает размер списка")
            }
        } else {
            Err("Индекс превышает размер списка")
        }
    }

    pub fn dell_before_index(&mut self, index: usize) -> Result<(), &'static str> {
        if index == 0 {
            return Err("Нельзя удалить элемент перед индексом 0");
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
            return Err("Список пустой");
        }

        if self.head.as_ref().unwrap().data == value {
            return self.dell_head();
        }

        let mut current = &mut self.head;
        while current.as_ref().unwrap().next.is_some() {
            if current.as_ref().unwrap().next.as_ref().unwrap().data == value {
                break;
            }
            current = &mut current.as_mut().unwrap().next;
        }

        if let Some(node) = current {
            if node.next.is_some() {
                let next_node = node.next.take().unwrap();
                node.set_next(next_node.next);
                Ok(())
            } else {
                Err("Значение отсутствует в списке")
            }
        } else {
            Err("Значение отсутствует в списке")
        }
    }

    pub fn search_value(&self, value: &str) -> Result<bool, &'static str> {
        if value.is_empty() {
            return Err("Значение пустое");
        }

        if self.head.is_none() {
            return Err("Список пустой");
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
                print!(" -> ");
            }
            current = &node.next;
        }
        println!();
    }
}

impl Drop for SimplyList {
    fn drop(&mut self) {
        while self.dell_head().is_ok() {}
    }
}

impl fmt::Display for SimplyList {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.head.is_none() {
            return write!(f, "(Список пустой)");
        }

        let mut current = &self.head;
        let mut first = true;

        while let Some(node) = current {
            if !first {
                write!(f, " -> ")?;
            }
            write!(f, "{}", node.data)?;
            current = &node.next;
            first = false;
        }

        Ok(())
    }
}