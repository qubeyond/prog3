use crate::e::Queue::*;
use std::fs;

#[test]
fn test_push_pop() {
    let mut q = Queue::new();
    q.push("A");
    q.push("B");
    assert_eq!(q.pop().unwrap(), "A");
    assert_eq!(q.pop().unwrap(), "B");
    assert!(q.pop().is_none());
    q.clear();
}

#[test]
fn test_peek_len() {
    let mut q = Queue::new();
    q.push("one");
    q.push("two");
    assert_eq!(q.peek().unwrap(), "one");
    assert_eq!(q.len(), 2);
    q.clear();
}

#[test]
fn test_file_io() {
    let mut q = Queue::new();
    q.push("x");
    q.push("y");
    q.write_to_file("test_queue.txt").unwrap();
    let mut q2 = Queue::new();
    q2.read_from_file("test_queue.txt").unwrap();
    assert_eq!(q2.len(), 2);
    assert_eq!(q2.peek().unwrap(), "x");
    q.clear();
    q2.clear();
    fs::remove_file("test_queue.txt").unwrap();
}

#[test]
fn test_clear() {
    let mut q = Queue::new();
    q.push("z");
    q.clear();
    assert_eq!(q.len(), 0);
}
