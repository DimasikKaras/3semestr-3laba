#![allow(dead_code)]
#![allow(unused_variables)]
#![allow(unused_imports)]
#![allow(unused_mut)]
#![allow(unused_assignments)]

pub mod structures;
pub mod command_parser;
pub mod storage;
// Подключаем модули
pub mod doubly_list;
pub mod stack;
pub mod dy_arr;
pub mod simply_list;
pub mod hash_table;
pub mod queue;

// Реэкспортируем основные типы для удобства использования
pub use structures::*;
pub use command_parser::process_commands;
pub use storage::Storage;

#[cfg(test)]
mod structures_tests;