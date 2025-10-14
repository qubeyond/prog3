use crate::c::DoublyLinkedList::*;
use std::fs;

#[test]
fn test_init() {
    let list = DLinkedList::new();
    assert!(list.head.is_none() && list.tail.is_none());
}

#[test]
fn test_add_head_tail() {
    let mut list = DLinkedList::new();
    list.add_head("10");
    list.add_tail("20");
    assert_eq!(list.head.as_ref().unwrap().borrow().value, "10");
    assert_eq!(list.tail.as_ref().unwrap().borrow().value, "20");
}

#[test]
fn test_add_before_after() {
    let mut list = DLinkedList::new();
    list.add_head("10");
    list.add_tail("30");
    assert!(list.add_before("30", "20"));
    assert!(list.add_after("10", "5"));
    assert!(!list.add_before("999", "0"));
    assert!(!list.add_after("999", "0"));
}

#[test]
fn test_remove() {
    let mut list = DLinkedList::new();
    list.add_head("10"); list.add_tail("20"); list.add_tail("30");
    assert!(list.remove_head());
    assert_eq!(list.head.as_ref().unwrap().borrow().value, "20");
    assert!(list.remove_tail());
    assert_eq!(list.tail.as_ref().unwrap().borrow().value, "20");
    list.add_tail("30");
    assert!(list.remove_value("30"));
    assert!(!list.remove_value("999"));
    list.add_head("10"); list.add_tail("20"); list.add_tail("30");
    assert!(list.remove_before("20"));  
    assert!(list.remove_after("20"));   
}

#[test]
fn test_get_find() {
    let mut list = DLinkedList::new();
    list.add_tail("10"); list.add_tail("20"); list.add_tail("30");
    assert_eq!(list.get_node(0).unwrap().borrow().value, "10");
    assert_eq!(list.get_node(1).unwrap().borrow().value, "20");
    assert_eq!(list.get_node(2).unwrap().borrow().value, "30");
    assert!(list.get_node(3).is_none());
    assert_eq!(list.find_value("20").unwrap().borrow().value, "20");
    assert!(list.find_value("999").is_none());
}

#[test]
fn test_print() {
    let mut list = DLinkedList::new();
    list.add_tail("1"); list.add_tail("2"); list.add_tail("3");
    list.print_list();
}

#[test]
fn test_file_io() {
    let mut list = DLinkedList::new();
    list.add_tail("100"); list.add_tail("200");
    list.write_to_file("test_dlist.txt").unwrap();
    let mut new_list = DLinkedList::new();
    new_list.read_from_file("test_dlist.txt").unwrap();
    assert_eq!(new_list.head.as_ref().unwrap().borrow().value, "100");
    assert_eq!(new_list.tail.as_ref().unwrap().borrow().value, "200");
    fs::remove_file("test_dlist.txt").unwrap();
}

#[test]
fn test_clear() {
    let mut list = DLinkedList::new();
    list.add_tail("1"); list.add_tail("2"); list.add_tail("3");
    list.clear_list();
    assert!(list.head.is_none() && list.tail.is_none());
}

#[test]
fn test_stress() {
    let mut list = DLinkedList::new();
    const N: usize = 10000;
    for i in 0..N {
        list.add_tail(&i.to_string());
    }
    assert_eq!(list.head.as_ref().unwrap().borrow().value, "0");
    assert_eq!(list.tail.as_ref().unwrap().borrow().value, (N-1).to_string());
    for _ in 0..N/2 {
        list.remove_head();
    }
    assert_eq!(list.head.as_ref().unwrap().borrow().value, (N/2).to_string());
    list.clear_list();
}
