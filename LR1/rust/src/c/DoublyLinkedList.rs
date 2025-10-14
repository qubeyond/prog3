use std::cell::RefCell;
use std::fs::File;
use std::io::{self, BufRead, Write};
use std::rc::Rc;

type NodeRef = Rc<RefCell<DNode>>;

#[derive(Debug)]
pub struct DNode {
    pub value: String,
    pub prev: Option<NodeRef>,
    pub next: Option<NodeRef>,
}

#[derive(Debug)]
pub struct DLinkedList {
    pub head: Option<NodeRef>,
    pub tail: Option<NodeRef>,
}

impl DLinkedList {
    pub fn new() -> Self {
        Self { head: None, tail: None }
    }

    pub fn add_head(&mut self, val: &str) {
        let node = Rc::new(RefCell::new(DNode { value: val.to_string(), prev: None, next: self.head.clone() }));
        if let Some(old_head) = &self.head {
            old_head.borrow_mut().prev = Some(node.clone());
        } else {
            self.tail = Some(node.clone());
        }
        self.head = Some(node);
    }

    pub fn add_tail(&mut self, val: &str) {
        let node = Rc::new(RefCell::new(DNode { value: val.to_string(), prev: self.tail.clone(), next: None }));
        if let Some(old_tail) = &self.tail {
            old_tail.borrow_mut().next = Some(node.clone());
        } else {
            self.head = Some(node.clone());
        }
        self.tail = Some(node);
    }

    pub fn add_before(&mut self, target: &str, val: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur.clone() {
            if node.borrow().value == target {
                let prev = node.borrow().prev.clone();
                let new_node = Rc::new(RefCell::new(DNode { value: val.to_string(), prev: prev.clone(), next: Some(node.clone()) }));
                node.borrow_mut().prev = Some(new_node.clone());
                if let Some(prev_node) = prev {
                    prev_node.borrow_mut().next = Some(new_node);
                } else {
                    self.head = Some(new_node);
                }
                return true;
            }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn add_after(&mut self, target: &str, val: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur.clone() {
            if node.borrow().value == target {
                let next = node.borrow().next.clone();
                let new_node = Rc::new(RefCell::new(DNode { value: val.to_string(), prev: Some(node.clone()), next: next.clone() }));
                node.borrow_mut().next = Some(new_node.clone());
                if let Some(next_node) = next {
                    next_node.borrow_mut().prev = Some(new_node);
                } else {
                    self.tail = Some(new_node);
                }
                return true;
            }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn remove_head(&mut self) -> bool {
        if let Some(old_head) = self.head.take() {
            let next = old_head.borrow_mut().next.take();
            if let Some(next_node) = &next {
                next_node.borrow_mut().prev = None;
            } else {
                self.tail = None;
            }
            self.head = next;
            true
        } else {
            false
        }
    }

    pub fn remove_tail(&mut self) -> bool {
        if let Some(old_tail) = self.tail.take() {
            let prev = old_tail.borrow_mut().prev.take();
            if let Some(prev_node) = &prev {
                prev_node.borrow_mut().next = None;
                self.tail = Some(prev_node.clone());
            } else {
                self.head = None;
            }
            true
        } else {
            false
        }
    }

    pub fn remove_value(&mut self, val: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur.clone() {
            if node.borrow().value == val {
                let prev = node.borrow_mut().prev.take();
                let next = node.borrow_mut().next.take();
                if let Some(prev_node) = prev.clone() {
                    prev_node.borrow_mut().next = next.clone();
                } else {
                    self.head = next.clone();
                }
                if let Some(next_node) = next.clone() {
                    next_node.borrow_mut().prev = prev;
                } else {
                    self.tail = prev;
                }
                return true;
            }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn remove_before(&mut self, target: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur.clone() {
            if node.borrow().value == target {
                if let Some(prev_node) = node.borrow().prev.clone() {
                    let prev_prev = prev_node.borrow().prev.clone();
                    if let Some(prev_prev_node) = &prev_prev {
                        prev_prev_node.borrow_mut().next = Some(node.clone());
                    } else {
                        self.head = Some(node.clone());
                    }
                    node.borrow_mut().prev = prev_prev;
                    return true;
                } else {
                    return false;
                }
            }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn remove_after(&mut self, target: &str) -> bool {
        let mut cur = self.head.clone();
        while let Some(node) = cur.clone() {
            if node.borrow().value == target {
                if let Some(next_node) = node.borrow().next.clone() {
                    let next_next = next_node.borrow().next.clone();
                    node.borrow_mut().next = next_next.clone();
                    if let Some(next_next_node) = next_next {
                        next_next_node.borrow_mut().prev = Some(node.clone());
                    } else {
                        self.tail = Some(node.clone());
                    }
                    return true;
                } else {
                    return false;
                }
            }
            cur = node.borrow().next.clone();
        }
        false
    }

    pub fn get_node(&self, index: usize) -> Option<NodeRef> {
        let mut cur = self.head.clone();
        for _ in 0..index {
            if let Some(node) = cur.clone() {
                cur = node.borrow().next.clone();
            } else {
                return None;
            }
        }
        cur
    }

    pub fn find_value(&self, val: &str) -> Option<NodeRef> {
        let mut cur = self.head.clone();
        while let Some(node) = cur.clone() {
            if node.borrow().value == val {
                return Some(node);
            }
            cur = node.borrow().next.clone();
        }
        None
    }

    pub fn print_list(&self) {
        let mut cur = self.head.clone();
        let mut first = true;
        while let Some(node) = cur {
            if !first { print!(" "); }
            print!("{}", node.borrow().value);
            first = false;
            cur = node.borrow().next.clone();
        }
        println!();
    }

    pub fn read_from_file(&mut self, filename: &str) -> io::Result<()> {
        self.clear_list();
        let file = File::open(filename)?;
        for line in io::BufReader::new(file).lines() {
            for token in line?.split_whitespace() {
                self.add_tail(token);
            }
        }
        Ok(())
    }

    pub fn write_to_file(&self, filename: &str) -> io::Result<()> {
        let mut file = File::create(filename)?;
        let mut cur = self.head.clone();
        let mut first = true;
        while let Some(node) = cur {
            if !first { write!(file, " ")?; }
            write!(file, "{}", node.borrow().value)?;
            first = false;
            cur = node.borrow().next.clone();
        }
        Ok(())
    }

    pub fn clear_list(&mut self) {
        self.head = None;
        self.tail = None;
    }
}
