mod b;
use b::LinkedList;

#[test]
fn test_init() {
    let list = LinkedList::new();
    assert!(list.head.is_none());
}

#[test]
fn test_add_head() {
    let mut list = LinkedList::new();
    list.add_head("10");
    assert_eq!(list.head.as_ref().unwrap().value, "10");
    list.add_head("20");
    assert_eq!(list.head.as_ref().unwrap().value, "20");
    assert_eq!(list.head.as_ref().unwrap().next.as_ref().unwrap().value, "10");
}

#[test]
fn test_add_tail() {
    let mut list = LinkedList::new();
    list.add_tail("5");
    assert_eq!(list.head.as_ref().unwrap().value, "5");
    list.add_tail("15");
    let next = list.head.as_ref().unwrap().next.as_ref().unwrap();
    assert_eq!(next.value, "15");
}

#[test]
fn test_add_before() {
    let mut list = LinkedList::new();
    list.add_head("10");
    list.add_head("20");
    assert!(list.add_before("10", "15"));
    assert_eq!(list.head.as_ref().unwrap().next.as_ref().unwrap().value, "15");
    assert!(list.add_before("20", "5"));
    assert_eq!(list.head.as_ref().unwrap().value, "5");
    assert!(!list.add_before("999", "50"));
}

#[test]
fn test_add_after() {
    let mut list = LinkedList::new();
    list.add_head("10");
    list.add_head("20");
    assert!(list.add_after("10", "15"));
    let val = list.head.as_ref().unwrap().next.as_ref().unwrap().next.as_ref().unwrap();
    assert_eq!(val.value, "15");
    assert!(!list.add_after("999", "50"));
}

#[test]
fn test_remove_head() {
    let mut list = LinkedList::new();
    assert!(!list.remove_head());
    list.add_head("10");
    assert!(list.remove_head());
    assert!(list.head.is_none());
    list.add_head("1");
    list.add_tail("2");
    list.add_tail("3");
    assert!(list.remove_head());
    assert_eq!(list.head.as_ref().unwrap().value, "2");
}

#[test]
fn test_remove_tail() {
    let mut list = LinkedList::new();
    assert!(!list.remove_tail());
    list.add_head("10");
    assert!(list.remove_tail());
    assert!(list.head.is_none());
    list.add_head("1");
    list.add_tail("2");
    list.add_tail("3");
    assert!(list.remove_tail());
    let mut cur = list.head.as_ref().unwrap();
    while let Some(next) = cur.next.as_ref() {
        cur = next;
    }
    assert_eq!(cur.value, "2");
}

#[test]
fn test_remove_value() {
    let mut list = LinkedList::new();
    assert!(!list.remove_value("10"));
    list.add_head("10");
    assert!(list.remove_value("10"));
    assert!(list.head.is_none());
    list.add_head("20");
    assert!(!list.remove_value("999"));
    list.add_head("1");
    list.add_tail("2");
    list.add_tail("3");
    assert!(list.remove_value("2"));
    assert!(list.remove_value("1"));
    assert!(list.remove_value("20"));
    assert!(!list.remove_value("999"));
}

#[test]
fn test_remove_before_after() {
    let mut list = LinkedList::new();
    list.add_tail("A");
    list.add_tail("B");
    list.add_tail("C");
    list.add_tail("D");
    assert!(list.remove_before("C")); // удаляем B
    assert_eq!(list.head.as_ref().unwrap().next.as_ref().unwrap().value, "C");
    assert!(list.remove_after("C"));  // удаляем D
    assert!(list.head.as_ref().unwrap().next.as_ref().unwrap().next.is_none());
    assert!(!list.remove_before("A"));
}

#[test]
fn test_print_and_clear() {
    let mut list = LinkedList::new();
    list.add_tail("1");
    list.add_tail("2");
    list.add_tail("3");
    list.print(); // просто проверим, что не падает
    list.clear();
    assert!(list.head.is_none());
}

#[test]
fn test_file_io() {
    let mut list = LinkedList::new();
    list.add_tail("100");
    list.add_tail("200");
    list.add_tail("300");
    list.write_to_file("test_list.txt").unwrap();
    let mut list2 = LinkedList::new();
    list2.read_from_file("test_list.txt").unwrap();
    let mut cur = list2.head.as_ref().unwrap();
    assert_eq!(cur.value, "100");
    cur = cur.next.as_ref().unwrap();
    assert_eq!(cur.value, "200");
    cur = cur.next.as_ref().unwrap();
    assert_eq!(cur.value, "300");
    std::fs::remove_file("test_list.txt").unwrap();
}

#[test]
fn test_stress() {
    let mut list = LinkedList::new();
    let n = 10_000;
    for i in 0..n {
        list.add_tail(&i.to_string());
    }

    assert_eq!(list.head.as_ref().unwrap().value, "0");
    let mut cur = list.head.as_ref();
    let mut count = 0;
    while let Some(node) = cur {
        count += 1;
        cur = node.next.as_ref();
    }
    assert_eq!(count, n);

    for i in 0..(n / 2) {
        assert!(list.remove_value(&i.to_string()));
    }

    let mut cur = list.head.as_ref();
    let mut expected = n / 2;
    while let Some(node) = cur {
        assert_eq!(node.value, expected.to_string());
        expected += 1;
        cur = node.next.as_ref();
    }
    assert_eq!(expected, n);
}
