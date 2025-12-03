#[cfg(test)]
mod extended_coverage_tests {
    use super::super::*;

    //==============Динамический массив==================
    #[test]
    fn test_dy_arr() {
        let mut arr = DyArr::new(2);

        arr.push_back("10");
        arr.push_back("20");
        arr.push_back("30");

        assert_eq!(arr.capacity, 4);

        assert!(arr.push_in_index("40",5).is_err());
        arr.push_in_index("20", 1).unwrap();

        assert!(arr.push_in_index("10", 2).is_ok());

        assert_eq!(arr.get_element(2).unwrap(), "10");
        assert!(arr.get_element(5).is_err());

        assert!(arr.remove_at_index(2).is_ok());
        assert!(arr.remove_at_index(20).is_err());

        assert!(arr.replace_at_index("199",2).is_ok());
        assert!(arr.replace_at_index("199",20).is_err());

        assert_eq!(arr.get_length(), 4);
        assert_eq!(arr.get_capacity(), 8);

        arr.print_dy_arr();
    }

    //================Стек==================
    #[test]
    fn test_stack() {
        let mut stack = Stack::new();
        stack.push("a");
        let top = stack.get_top().unwrap();
        assert!(top.get_next().is_none());
        stack.pop();
        assert!(stack.pop().is_err());



        stack.push("10");
        stack.push("20");
        stack.push("30");
        stack.push("40");
        stack.print();
        let display_output = format!("{}", stack);
        assert_eq!(display_output, "40 -> 30 -> 20 -> 10");


        assert_eq!(stack.get_top().unwrap().get_data(), "40");
        let mut stack = Stack::new();
        let display_output = format!("{}", stack);
        assert_eq!(display_output, "Пустой стек");
    }


    //================Очередь==================
    #[test]
    fn test_queue() {
        let mut queue = Queue::new();

        assert!(queue.dequeue().is_err());

        queue.enqueue("A1");
        queue.enqueue("A2");
        queue.enqueue("A3");
        queue.enqueue("A4");
        queue.print();
        let display_output = format!("{}", queue);
        assert_eq!(display_output, "A1 -> A2 -> A3 -> A4");

        assert_eq!(queue.get_first().unwrap().get_data(), "A1");

        let mut queue = Queue::new();
        let display_output = format!("{}", queue);
        assert_eq!(display_output, "(Пустая очередь)");
        queue.print();
        queue.enqueue("first");
        queue.enqueue("second");
        queue.enqueue("third");

        // Вызываем rebuild_pointers
        queue.rebuild_pointers();
    }

    //================Открытая адресация==============

    #[test]
    fn open_addressing(){
        let mut table = OpenAddressingHashTable::new(2);

        assert!(table.insert("","a").is_err());
        assert!(table.insert("a","").is_err());

        assert!(table.insert("aa","8322").is_ok());
        assert!(table.insert("aa","813231").is_ok());
        assert!(table.insert("ab","813231").is_ok());
        assert!(table.delete("ab").is_ok());
        assert!(table.insert("ab","813231").is_ok());
        assert!(table.delete("").is_err());
        assert!(table.delete("aaaa").is_ok());

        assert!(table.insert("asjklkd","813231").is_ok());
        assert!(table.insert("adfds","813231").is_ok());
        assert!(table.insert("aq","813231").is_ok());

        assert!(table.delete("aq").is_ok());

        assert!(table.search("adfds").is_ok());
        assert!(table.search("").is_err());
        assert_eq!(table.get_size(), 4);
        assert_eq!(table.get_capacity(), 7);
        table.print();

        let mut table = OpenAddressingHashTable::new(10);
        let display_output = format!("{}", table);
        assert_eq!(display_output, "OpenAddressingHashTable(0/10)");
        table.insert("k1", "v1").unwrap();
        table.insert("k2", "v2").unwrap();

        let elements = table.get_all_elements();
        assert_eq!(elements.len(), 2);
        assert!(elements.contains(&("k1".to_string(), "v1".to_string())));
    }

    //================Метод цепочек==============
    #[test]
    fn chain_table(){
        let mut table = ChainHashTable::new(2);

        assert!(table.insert("","a").is_err());
        assert!(table.insert("a","").is_err());

        assert!(table.insert("aa","8322").is_ok());
        assert!(table.insert("aa","813231").is_ok());
        assert!(table.insert("ab","813231").is_ok());
        assert!(table.delete("ab").is_ok());
        assert!(table.insert("ab","813231").is_ok());
        assert!(table.delete("").is_err());
        assert!(table.delete("aaaa").is_ok());

        assert!(table.insert("asjklkd","813231").is_ok());
        assert!(table.insert("adfds","813231").is_ok());
        assert!(table.insert("aq","813231").is_ok());

        assert!(table.delete("aq").is_ok());

        assert!(table.search("adfds").is_ok());
        assert!(table.search("").is_err());
        assert_eq!(table.get_size(), 4);
        assert_eq!(table.get_capacity(), 7);
        table.print();

        let mut table = ChainHashTable::new(10);
        let display_output = format!("{}", table);
        assert_eq!(display_output, "ChainHashTable(0/10)");
        table.insert("k1", "v1").unwrap();
        table.insert("k2", "v2").unwrap();

        let elements = table.get_all_elements();
        assert_eq!(elements.len(), 2);
        assert!(elements.contains(&("k1".to_string(), "v1".to_string())));
    }

    //================Односвязный список==============
    #[test]
    fn simply_list(){
        let mut list = SimplyList::new();

        assert_eq!(format!("{}", list), "(Список пустой)");
        assert!(list.add_back("").is_err());
        assert!(list.add_head("").is_err());
        assert!(list.add_after_index(0, "").is_err());
        assert!(list.add_before_index(0, "").is_err());
        assert!(list.dell_value("").is_err());
        assert!(list.search_value("").is_err());

        assert!(list.add_back("1").is_ok());
        assert!(list.add_back("2").is_ok());
        assert!(list.add_back("3").is_ok());
        assert!(list.dell_back().is_ok());
        assert!(list.dell_back().is_ok());
        assert!(list.dell_back().is_ok());

        assert!(list.add_head("0").is_ok());

        assert!(list.add_after_index(0,"0").is_ok());
        assert!(list.add_after_index(1,"2").is_ok());

        assert!(list.add_before_index(0,"3").is_ok());
        assert!(list.add_before_index(2,"3").is_ok());
        assert!(list.dell_after_index(2).is_ok());
        assert!(list.dell_before_index(1).is_ok());
        assert!(list.dell_before_index(3).is_ok());
        assert!(list.dell_value("0").is_ok());




        assert!(list.add_back("1").is_ok());
        assert!(list.add_back("2").is_ok());
        assert!(list.add_back("3").is_ok());
        assert!(list.dell_value("321").is_err());
        assert!(list.search_value("321").is_ok());
        assert!(list.search_value("3").is_ok());
        list.print();


        let mut list = SimplyList::new();
        list.print();
        assert!(list.dell_back().is_err());
        assert!(list.dell_head().is_err());
        assert!(list.dell_after_index(0).is_err());
        assert!(list.dell_before_index(0).is_err());
        assert!(list.dell_value("0").is_err());
        assert!(list.search_value("0").is_err());

        assert!(list.add_after_index(1, " ").is_err());
        assert!(list.add_after_index(0, "1").is_ok());
        assert!(list.add_after_index(30, "1").is_err());
        assert_eq!(format!("{}", list), "1");


        let mut list = SimplyList::new();
        list.add_head("test").unwrap();

        // get_head и навигация по узлам
        let head = list.get_head().unwrap();
        assert_eq!(head.get_data(), "test");
        assert!(head.get_next().is_none());

        // С несколькими элементами
        list.add_head("new_head").unwrap();
        let new_head = list.get_head().unwrap();
        assert_eq!(new_head.get_data(), "new_head");

        let next = new_head.get_next().unwrap();
        assert_eq!(next.get_data(), "test");
        assert!(next.get_next().is_none());

    }

    #[test]
    fn doubly_list(){
        let mut list = DoublyList::new();

        assert_eq!(format!("{}", list), "(Список пустой)");
        assert!(list.add_back("").is_err());
        assert!(list.add_head("").is_err());
        assert!(list.add_after_index(0, "").is_err());
        assert!(list.add_before_index(0, "").is_err());
        assert!(list.dell_value("").is_err());
        assert!(list.search_value("").is_err());

        assert!(list.add_back("1").is_ok());
        assert!(list.add_back("2").is_ok());
        assert!(list.add_back("3").is_ok());
        assert!(list.dell_back().is_ok());
        assert!(list.dell_back().is_ok());
        assert!(list.dell_back().is_ok());

        assert!(list.add_head("0").is_ok());

        assert!(list.add_after_index(0,"0").is_ok());
        assert!(list.add_after_index(1,"2").is_ok());

        assert!(list.add_before_index(0,"3").is_ok());
        assert!(list.add_before_index(2,"3").is_ok());
        assert!(list.dell_after_index(2).is_ok());
        assert!(list.dell_before_index(1).is_ok());
        assert!(list.dell_before_index(3).is_ok());
        assert!(list.dell_value("0").is_ok());




        assert!(list.add_back("1").is_ok());
        assert!(list.add_back("2").is_ok());
        assert!(list.add_back("3").is_ok());
        assert!(list.dell_value("321").is_ok());
        assert!(list.search_value("321").is_ok());
        assert!(list.search_value("3").is_ok());
        list.print();
        list.print_reverse();


        let mut list = DoublyList::new();
        list.print();
        assert!(list.dell_back().is_err());
        assert!(list.dell_head().is_err());
        assert!(list.dell_after_index(0).is_err());
        assert!(list.dell_before_index(0).is_err());
        assert!(list.dell_value("0").is_ok());

        assert!(list.add_after_index(1, " ").is_err());
        assert!(list.add_after_index(0, "1").is_ok());
        assert!(list.add_after_index(30, "1").is_err());
        assert_eq!(format!("{}", list), "1");


        let mut list = DoublyList::new();
        list.add_head("test").unwrap();
        let head = list.get_head().unwrap();
        list.add_head("new_head").unwrap();
        let new_head = list.get_head().unwrap();
        let next = new_head.get_next().unwrap();
        assert!(list.dell_value("new_head1").is_ok());

        let mut list = DoublyList::new();

        // Пустое значение
        assert!(list.dell_value("").is_err());

        // Пустой список
        assert!(list.dell_value("test").is_ok());

        // Удаление головы
        list.add_head("head").unwrap();
        list.add_back("tail").unwrap();
        assert!(list.dell_value("head").is_ok());
        assert_eq!(list.get_head().unwrap().get_data(), "tail");

        // Удаление хвоста
        list.add_head("new_head").unwrap();
        assert!(list.dell_value("tail").is_ok());

        // Удаление несуществующего значения
        assert!(list.dell_value("missing").is_ok());

        let mut list = DoublyList::new();


        // С одним элементом
        list.add_head("a").unwrap();
        list.rebuild_pointers();
        assert_eq!(list.get_head().unwrap().get_data(), "a");

        // С несколькими элементами
        list.add_back("b").unwrap();
        list.add_back("c").unwrap();
        list.rebuild_pointers();

        // Проверяем навигацию после перестройки указателей
        let head = list.get_head().unwrap();
        assert_eq!(head.get_data(), "a");

        let second = head.get_next().unwrap();
        assert_eq!(second.get_data(), "b");

        let third = second.get_next().unwrap();
        assert_eq!(third.get_data(), "c");
        assert!(third.get_next().is_none());
    }
}

