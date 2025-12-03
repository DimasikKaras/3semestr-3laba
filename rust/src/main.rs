mod structures;
mod command_parser;
mod storage;
// Подключаем модули
mod doubly_list;
mod stack;
mod dy_arr;
mod simply_list;
mod hash_table;
mod queue;

use clap::{Arg, Command};
use std::process;

fn main() {
    let matches = Command::new("Data Structures CLI")
        .version("1.0")
        .author("Your Name")
        .about("Manages various data structures")
        .arg(
            Arg::new("file")
                .short('f')
                .long("file")
                .value_name("FILENAME")
                .help("Input file to use")
                .required(true)
        )
        .arg(
            Arg::new("binary")
                .short('b')
                .long("binary")
                .value_name("BINARY_FILENAME")
                .help("Binary file to save/load data")
        )
        .arg(
            Arg::new("query")
                .short('q')
                .long("query")
                .value_name("QUERY")
                .help("Command to execute")
                .required(true)
        )
        .get_matches();

    let filename = matches.get_one::<String>("file").unwrap();
    let query = matches.get_one::<String>("query").unwrap();
    let binary_filename = matches.get_one::<String>("binary");

    // Загружаем данные из текстового файла или создаем новое хранилище
    let mut storage = storage::Storage::load_from_text(filename).unwrap_or_else(|_| storage::Storage::new());

    // Обрабатываем команды
    match command_parser::process_commands(&mut storage, query) {
        Ok(_) => {
            // Сохраняем данные в текстовый файл
            if let Err(e) = storage.save_to_text(filename) {
                eprintln!("Ошибка сохранения в файл: {}", e);
                process::exit(1);
            }

            // Сохраняем в бинарный файл если указан
            if let Some(bin_filename) = binary_filename {
                if let Err(e) = storage.save_to_binary(bin_filename) {
                    eprintln!("Ошибка сохранения в бинарный файл: {}", e);
                    process::exit(1);
                }
                println!("Данные сохранены в бинарный файл: {}", bin_filename);
            }
        }
        Err(e) => {
            eprintln!("Ошибка выполнения команды: {}", e);
            process::exit(1);
        }
    }
}

fn print_help() {
    println!("=========Использование программы========");
    println!("cargo run -- -f <filename> -q \"команда\"");
    println!("=================Ключи==================");
    println!("-h, --help          показать эту справку");
    println!("-f, --file <arg>    использование файла arg");
    println!("-b, --binary <arg>  сохранение в бинарный файл arg");
    println!("-q, --query <arg>   выполнение команды arg");
    println!();
    println!("============Создание структур===========");
    println!("MCREATE <name>              создать массив");
    println!("FCREATE <name>              создать односвязный список");
    println!("DCREATE <name>              создать двусвязный список");
    println!("SCREATE <name>              создать стек");
    println!("QCREATE <name>              создать очередь");
    println!("TABLEOPENCREATE <name>      создать хэш таблицу открытой адресации");
    println!("TABLECHAINCREATE <name>     создать хэш таблицу методом цепочек");
    println!();
    println!("==========Операции с массивом==========");
    println!("MPUSH <name> <value>              добавить в конец");
    println!("MPUSHINDEX <name> <index> <value> добавить в индекс");
    println!("MGET <name> <index>               получение значения по индексу");
    println!("MDELINDEX <name> <index>          удаление по индексу");
    println!("MREPLACE <name> <index> <value>   замена по индексу");
    println!("MLEN <name>                       получить длину массива");
    println!();
    println!("=====Операции с односвязным списком====");
    println!("FPUSHHEAD <name> <value>          добавить в начало");
    println!("FPUSHTAIL <name> <value>          добавить в конец");
    println!("FPUSHAFTER <name> <index> <value> добавить после индекса");
    println!("FPUSHBEFORE <name> <index> <value> добавить до индекса");
    println!("FDELHEAD <name>                   удалить из начала");
    println!("FDELTAIL <name>                   удалить из конца");
    println!("FDELAFTER <name> <index>          удалить после индекса");
    println!("FDELBEFORE <name> <index>         удалить до индекса");
    println!("FDELVALUE <name> <value>          удалить по значению");
    println!("FSEARCH <name> <value>            найти элемент");
    println!();
    println!("=====Операции с двусвязным списком=====");
    println!("DPUSHHEAD <name> <value>          добавить в начало");
    println!("DPUSHTAIL <name> <value>          добавить в конец");
    println!("DPUSHAFTER <name> <index> <value> добавить после индекса");
    println!("DPUSHBEFORE <name> <index> <value> добавить до индекса");
    println!("DDELHEAD <name>                   удалить из начала");
    println!("DDELTAIL <name>                   удалить из конца");
    println!("DDELAFTER <name> <index>          удалить после индекса");
    println!("DDELBEFORE <name> <index>         удалить до индекса");
    println!("DDELVALUE <name> <value>          удалить по значению");
    println!("DSEARCH <name> <value>            найти элемент");
    println!();
    println!("=========Операции со стеком============");
    println!("SPUSH <name> <value>              добавить");
    println!("SPOP <name>                       удалить и получить");
    println!();
    println!("=========Операции с очередью===========");
    println!("QPUSH <name> <value>              добавить");
    println!("QPOP <name>                       удалить и получить");
    println!();
    println!("===Хэш таблица (открытая адресация)====");
    println!("TABLEOPENADD <name> <key> <value> добавить элемент");
    println!("TABLEOPENDEL <name> <key>         удалить элемент");
    println!("TABLEOPENGET <name> <key>         найти элемент");
    println!();
    println!("=====Хэш таблица (метод цепочек)======");
    println!("TABLECHAINADD <name> <key> <value> добавить элемент");
    println!("TABLECHAINDEL <name> <key>         удалить элемент");
    println!("TABLECHAINGET <name> <key>         найти элемент");
    println!();
    println!("============Общие команды=============");
    println!("PRINT <name>                       вывести структуру");
    println!();
    println!("=============Примеры==================");
    println!("cargo run -- -f data.txt -q \"MCREATE arr1\"");
    println!("cargo run -- -f data.txt -q \"MPUSH arr1 hello\"");
    println!("cargo run -- -f data.txt -q \"PRINT arr1\"");
    println!("cargo run -- -f data.txt -b data.bin -q \"FCREATE list1; FPUSHHEAD list1 world; PRINT list1\"");
}