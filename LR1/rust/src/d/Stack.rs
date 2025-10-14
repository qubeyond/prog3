use std::fs::File;
use std::io::{self, BufRead, Write};

const MIN_CAPACITY: usize = 4;

pub struct Stack {
    data: Vec<String>,
}

impl Stack {
    pub fn new() -> Self {
        Stack {
            data: Vec::with_capacity(MIN_CAPACITY),
        }
    }

    fn ensure_capacity(&mut self) {
        if self.data.len() == self.data.capacity() {
            self.data.reserve(self.data.capacity());
        }
    }

    fn shrink_if_needed(&mut self) {
        if self.data.capacity() > MIN_CAPACITY && self.data.len() * 4 < self.data.capacity() {
            let new_cap = self.data.capacity() / 2;
            self.data.shrink_to(new_cap.max(MIN_CAPACITY));
        }
    }

    pub fn push(&mut self, val: &str) {
        self.ensure_capacity();
        self.data.push(val.to_string());
    }

    pub fn pop(&mut self) -> Option<String> {
        let val = self.data.pop();
        self.shrink_if_needed();
        val
    }

    pub fn top(&self) -> Option<&String> {
        self.data.last()
    }

    pub fn len(&self) -> usize {
        self.data.len()
    }

    pub fn clear(&mut self) {
        self.data.clear();
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
        for (i, val) in self.data.iter().enumerate() {
            if i > 0 { write!(file, " ")?; }
            write!(file, "{}", val)?;
        }
        Ok(())
    }
}
