use crate::d::Stack::*;
use std::fs;

#[test]
fn test_push_pop() {
    let mut s = Stack::new();
    s.push("A");
    s.push("B");
    assert_eq!(s.pop().unwrap(), "B");
    assert_eq!(s.pop().unwrap(), "A");
    assert!(s.pop().is_none());
    s.clear();
}

#[test]
fn test_top_len() {
    let mut s = Stack::new();
    s.push("X");
    s.push("Y");
    assert_eq!(s.top().unwrap(), "Y");
    assert_eq!(s.len(), 2);
    s.clear();
}

#[test]
fn test_file_io() {
    let mut s = Stack::new();
    s.push("one");
    s.push("two");
    s.write_to_file("test_stack.txt").unwrap();
    let mut s2 = Stack::new();
    s2.read_from_file("test_stack.txt").unwrap();
    assert_eq!(s2.len(), 2);
    assert_eq!(s2.top().unwrap(), "two");
    s.clear();
    s2.clear();
    fs::remove_file("test_stack.txt").unwrap();
}

#[test]
fn test_clear() {
    let mut s = Stack::new();
    s.push("z");
    s.clear();
    assert_eq!(s.len(), 0);
}
