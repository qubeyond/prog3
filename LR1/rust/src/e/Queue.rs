use std::fs::File;
use std::io::{self, BufRead, Write};

const MIN_CAPACITY: usize = 4;

pub struct Queue {
    data: Vec<String>,
    front: usize,
    back: usize,
}

impl Queue {
    pub fn new() -> Self {
        Queue {
            data: Vec::with_capacity(MIN_CAPACITY),
            front: 0,
            back: 0,
        }
    }

    fn ensure_capacity(&mut self) {
        if self.len() < self.data.capacity() {
            return;
        }
        let new_cap = self.data.capacity() * 2;
        let mut new_data = Vec::with_capacity(new_cap);
        for i in 0..self.len() {
            new_data.push(self.data[(self.front + i) % self.len()].clone());
        }
        self.data = new_data;
        self.front = 0;
        self.back = self.len();
    }

    fn shrink_if_needed(&mut self) {
        let len = self.len();
        if self.data.capacity() <= MIN_CAPACITY || len * 4 >= self.data.capacity() {
            return;
        }
        let new_cap = (self.data.capacity() / 2).max(MIN_CAPACITY);
        let mut new_data = Vec::with_capacity(new_cap);
        for i in 0..len {
            new_data.push(self.data[(self.front + i) % len].clone());
        }
        self.data = new_data;
        self.front = 0;
        self.back = len;
    }

    pub fn push(&mut self, val: &str) {
        self.ensure_capacity();
        if self.data.len() < self.data.capacity() {
            self.data.push(val.to_string());
        } else {
            self.data[self.back] = val.to_string();
        }
        self.back = (self.back + 1) % self.data.capacity();
    }

    pub fn pop(&mut self) -> Option<String> {
        if self.len() == 0 {
            return None;
        }
        let val = self.data[self.front].clone();
        self.front = (self.front + 1) % self.data.capacity();
        self.shrink_if_needed();
        Some(val)
    }

    pub fn peek(&self) -> Option<&String> {
        if self.len() == 0 {
            None
        } else {
            Some(&self.data[self.front])
        }
    }

    pub fn len(&self) -> usize {
        if self.back >= self.front {
            self.back - self.front
        } else {
            self.data.capacity() - self.front + self.back
        }
    }

    pub fn clear(&mut self) {
        self.data.clear();
        self.front = 0;
        self.back = 0;
    }

    pub fn read_from_file(&mut self, filename: &str) -> io::Result<()> {
        self.clear();
        let file = File::open(filename)?;
        for line in io::BufReader::new(file).lines() {
            for token in line?.split_whitespace() {
                self.push(token);
            }
        }
        Ok(())
    }

    pub fn write_to_file(&self, filename: &str) -> io::Result<()> {
        let mut file = File::create(filename)?;
        for i in 0..self.len() {
            if i > 0 {
                write!(file, " ")?;
            }
            write!(file, "{}", self.data[(self.front + i) % self.data.capacity()])?;
        }
        Ok(())
    }
}
