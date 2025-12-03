use std::fmt;

#[derive(serde::Serialize, serde::Deserialize, Clone, Debug)]
pub struct DyArr {
    pub(crate) data: Vec<String>,
    pub capacity: usize,
    pub length: usize,
}

impl DyArr {
    pub fn new(initial_capacity: usize) -> Self {
        DyArr {
            data: Vec::with_capacity(initial_capacity),
            capacity: initial_capacity,
            length: 0,
        }
    }

    pub fn push_back(&mut self, value: &str) {
        if self.length >= self.capacity {
            self.capacity *= 2;
            self.data.reserve(self.capacity - self.data.capacity());
        }
        self.data.push(value.to_string());
        self.length = self.data.len();
    }

    pub fn push_in_index(&mut self, value: &str, index: usize) -> Result<(), &'static str> {
        if index > self.length {
            return Err("ошибка индекса");
        }

        if self.length >= self.capacity {
            self.capacity *= 2;
            self.data.reserve(self.capacity - self.data.capacity());
        }

        self.data.insert(index, value.to_string());
        self.length = self.data.len();
        Ok(())
    }

    pub fn get_element(&self, index: usize) -> Result<&str, &'static str> {
        if index >= self.length {
            return Err("ошибка индекса");
        }
        Ok(&self.data[index])
    }

    pub fn remove_at_index(&mut self, index: usize) -> Result<(), &'static str> {
        if index >= self.length {
            return Err("ошибка индекса");
        }
        self.data.remove(index);
        self.length = self.data.len();
        Ok(())
    }

    pub fn replace_at_index(&mut self, value: &str, index: usize) -> Result<(), &'static str> {
        if index >= self.length {
            return Err("ошибка индекса");
        }
        self.data[index] = value.to_string();
        Ok(())
    }

    pub fn get_length(&self) -> usize {
        self.length
    }

    pub fn get_capacity(&self) -> usize {
        self.capacity
    }

    pub fn print_dy_arr(&self) {
        if self.length == 0 {
            println!("Пустой массив");
            return;
        }
        println!("{}", self);
    }
}

impl fmt::Display for DyArr {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        if self.length == 0 {
            return write!(f, "[]");
        }

        write!(f, "[")?;
        for (i, item) in self.data.iter().take(self.length).enumerate() {
            if i > 0 {
                write!(f, ", ")?;
            }
            write!(f, "{}", item)?;
        }
        write!(f, "]")
    }
}

// Реализация Drop для явного освобождения ресурсов
impl Drop for DyArr {
    fn drop(&mut self) {
        // Rust автоматически освободит память,
        // но можно добавить логику очистки если нужно
        self.data.clear();
        self.length = 0;
    }
}