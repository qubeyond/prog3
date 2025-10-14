use std::fs::{File, OpenOptions};
use std::io::{self, BufRead, Write};

#[derive(Debug)]
pub struct Node {
    pub value: String,
    pub next: Option<Box<Node>>,
}

#[derive(Debug)]
pub struct LinkedList {
    pub head: Option<Box<Node>>,
}

impl LinkedList {
    pub fn new() -> Self {
        LinkedList { head: None }
    }

    pub fn add_head(&mut self, val: &str) {
        let new_node = Box::new(Node { value: val.to_string(), next: self.head.take() });
        self.head = Some(new_node);
    }

    pub fn add_tail(&mut self, val: &str) {
        let new_node = Box::new(Node { value: val.to_string(), next: None });
        match self.head.as_mut() {
            None => self.head = Some(new_node),
            Some(mut cur) => {
                while let Some(next) = cur.next.as_mut() {
                    cur = next;
                }
                cur.next = Some(new_node);
            }
        }
    }

    pub fn add_before(&mut self, target: &str, val: &str) -> bool {
        if self.head.is_none() { return false; }
        if self.head.as_ref().unwrap().value == target {
            self.add_head(val);
            return true;
        }

        let mut cur = self.head.as_mut();
        while let Some(node) = cur {
            if let Some(next) = node.next.as_mut() {
                if next.value == target {
                    let new_node = Box::new(Node {
                        value: val.to_string(),
                        next: node.next.take(),
                    });
                    node.next = Some(new_node);
                    return true;
                }
            }
            cur = node.next.as_mut();
        }
        false
    }

    pub fn add_after(&mut self, target: &str, val: &str) -> bool {
        let mut cur = self.head.as_mut();
        while let Some(node) = cur {
            if node.value == target {
                let new_node = Box::new(Node {
                    value: val.to_string(),
                    next: node.next.take(),
                });
                node.next = Some(new_node);
                return true;
            }
            cur = node.next.as_mut();
        }
        false
    }

    pub fn remove_head(&mut self) -> bool {
        if self.head.is_none() { return false; }
        self.head = self.head.as_mut().and_then(|node| node.next.take());
        true
    }

    pub fn remove_tail(&mut self) -> bool {
        let mut cur = self.head.as_mut();
        if cur.is_none() { return false; }

        if cur.as_ref().unwrap().next.is_none() {
            self.head = None;
            return true;
        }

        while let Some(node) = cur {
            if node.next.as_ref().map(|n| n.next.is_none()).unwrap_or(false) {
                node.next = None;
                return true;
            }
            cur = node.next.as_mut();
        }
        false
    }

    pub fn remove_value(&mut self, val: &str) -> bool {
        if self.head.is_none() { return false; }

        if self.head.as_ref().unwrap().value == val {
            return self.remove_head();
        }

        let mut cur = self.head.as_mut();
        while let Some(node) = cur {
            if let Some(next) = node.next.as_mut() {
                if next.value == val {
                    node.next = next.next.take();
                    return true;
                }
            }
            cur = node.next.as_mut();
        }
        false
    }

    pub fn remove_before(&mut self, target: &str) -> bool {
        if self.head.is_none() || self.head.as_ref().unwrap().value == target {
            return false;
        }

        if let Some(h2) = self.head.as_ref().and_then(|h| h.next.as_ref()) {
            if h2.value == target {
                return self.remove_head();
            }
        }

        let mut cur = self.head.as_mut();
        while let Some(node) = cur {
            if let Some(n1) = node.next.as_mut() {
                if let Some(n2) = n1.next.as_ref() {
                    if n2.value == target {
                        node.next = n1.next.take();
                        return true;
                    }
                }
            }
            cur = node.next.as_mut();
        }
        false
    }

    pub fn remove_after(&mut self, target: &str) -> bool {
        let mut cur = self.head.as_mut();
        while let Some(node) = cur {
            if node.value == target {
                if let Some(mut next) = node.next.take() {
                    node.next = next.next.take();
                    return true;
                } else {
                    return false;
                }
            }
            cur = node.next.as_mut();
        }
        false
    }

    pub fn print(&self) {
        let mut cur = self.head.as_ref();
        while let Some(node) = cur {
            print!("{}", node.value);
            if node.next.is_some() {
                print!(" ");
            }
            cur = node.next.as_ref();
        }
        println!();
    }

    pub fn read_from_file(&mut self, filename: &str) -> io::Result<()> {
        self.clear();
        let file = File::open(filename)?;
        for line in io::BufReader::new(file).lines() {
            for val in line?.split_whitespace() {
                self.add_tail(val);
            }
        }
        Ok(())
    }

    pub fn write_to_file(&self, filename: &str) -> io::Result<()> {
        let mut file = OpenOptions::new().create(true).write(true).truncate(true).open(filename)?;
        let mut cur = self.head.as_ref();
        while let Some(node) = cur {
            write!(file, "{}", node.value)?;
            if node.next.is_some() {
                write!(file, " ")?;
            }
            cur = node.next.as_ref();
        }
        Ok(())
    }

    pub fn clear(&mut self) {
        self.head = None;
    }
}
