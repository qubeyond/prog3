use std::cmp::max;
use std::fs::{File, OpenOptions};
use std::io::{self, BufRead, Write};

const MIN_CAPACITY: usize = 10;

pub struct Array {
    pub data: Vec<String>,
    pub capacity: usize,
}

impl Array {
    pub fn new(initial_capacity: usize) -> Self {
        let cap = max(initial_capacity, MIN_CAPACITY);
        Array { data: Vec::with_capacity(cap), capacity: cap }
    }

    fn ensure_capacity(&mut self) {
        if self.data.len() >= self.capacity {
            self.capacity *= 2;
            self.data.reserve(self.capacity - self.data.capacity());
        }
    }

    fn shrink_capacity(&mut self) {
        if self.capacity > MIN_CAPACITY && self.data.len() < self.capacity / 4 {
            let new_cap = max(self.capacity / 2, MIN_CAPACITY);
            self.data.shrink_to(new_cap);
            self.capacity = new_cap;
        }
    }

    pub fn add_end(&mut self, val: &str) {
        self.ensure_capacity();
        self.data.push(val.to_string());
    }

    pub fn add_at(&mut self, index: usize, val: &str) -> bool {
        if index > self.data.len() { return false; }
        self.ensure_capacity();
        self.data.insert(index, val.to_string());
        true
    }

    pub fn get(&self, index: usize) -> Option<&String> {
        self.data.get(index)
    }

    pub fn set(&mut self, index: usize, val: &str) -> bool {
        if index >= self.data.len() { return false; }
        self.data[index] = val.to_string();
        true
    }

    pub fn remove_at(&mut self, index: usize) -> bool {
        if index >= self.data.len() { return false; }
        self.data.remove(index);
        self.shrink_capacity();
        true
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn clear(&mut self) {
        self.data.clear();
        self.capacity = MIN_CAPACITY;
    }

    pub fn read_from_file(&mut self, filename: &str) -> io::Result<()> {
        self.clear();
        let file = File::open(filename)?;
        for line in io::BufReader::new(file).lines() {
            for token in line?.split_whitespace() {
                self.add_end(token);
            }
        }
        Ok(())
    }

    pub fn write_to_file(&self, filename: &str) -> io::Result<()> {
        let mut file = OpenOptions::new().create(true).write(true).truncate(true).open(filename)?;
        for (i, val) in self.data.iter().enumerate() {
            if i > 0 { write!(file, " ")?; }
            write!(file, "{}", val)?;
        }
        Ok(())
    }
}
