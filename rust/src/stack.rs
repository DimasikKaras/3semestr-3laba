use std::fmt;
use serde::{Deserialize, Serialize};

#[derive(Clone, Serialize, Deserialize)]
pub struct StackNode {
    data: String,
    next: Option<Box<StackNode>>,
}

impl StackNode {
    pub fn new(value: &str) -> Self {
        StackNode {
            data: value.to_string(),
            next: None,
        }
    }

    pub fn get_data(&self) -> &str {
        &self.data
    }

    pub fn get_next(&self) -> Option<&StackNode> {
        self.next.as_deref()
    }

    pub fn set_next(&mut self, node: Option<Box<StackNode>>) {
        self.next = node;
    }
}
#[derive(Clone, Serialize, Deserialize)]
pub struct Stack {
    top: Option<Box<StackNode>>,
}

impl Stack {
    pub fn new() -> Self {
        Stack { top: None }
    }

    pub fn push(&mut self, value: &str) {
        let mut new_node = Box::new(StackNode::new(value));

        if let Some(old_top) = self.top.take() {
            new_node.set_next(Some(old_top));
        }

        self.top = Some(new_node);
    }

    pub fn pop(&mut self) -> Result<String, &'static str> {
        match self.top.take() {
            Some(node) => {
                self.top = node.next;
                Ok(node.data)
            }
            None => Err("Стек пустой"),
        }
    }

    pub fn print(&self) {
        if self.top.is_none() {
            println!("Пустой стек");
            return;
        }

        let mut current = &self.top;
        while let Some(node) = current {
            print!("{} ", node.get_data());
            current = &node.next;
        }
        println!();
    }

    pub fn get_top(&self) -> Option<&StackNode> {
        self.top.as_deref()
    }
}

impl Drop for Stack {
    fn drop(&mut self) {
        while self.pop().is_ok() {}
    }
}

impl fmt::Display for Stack {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.top.is_none() {
            return write!(f, "Пустой стек");
        }

        let mut current = &self.top;
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