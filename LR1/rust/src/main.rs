use clap::{Parser, Subcommand};
mod a;
mod b;
mod c;
mod d;
mod e;

use a::Array;
use b::LinkedList;
use c::DLinkedList;
use d::Stack;
use e::Queue;

const ARRAY_FILE: &str = "array.txt";
const STACK_FILE: &str = "stack.txt";
const QUEUE_FILE: &str = "queue.txt";
const LL_FILE: &str = "list.txt";
const DLL_FILE: &str = "dlist.txt";

#[derive(Parser)]
#[command(name = "Data Structures CLI", version, about = "Manage multiple data structures")]
struct Cli {
    #[command(subcommand)]
    ds: DataStructure,
}

#[derive(Subcommand)]
enum DataStructure {
    Array(ArrayCmd),
    Stack(StackCmd),
    Queue(QueueCmd),
    LinkedList(LLCmd),
    DLinkedList(DLLCmd),
}

#[derive(Subcommand)]
enum ArrayCmd {
    Print,
    Clear,
    AddEnd { val: String },
    Add { index: usize, val: String },
    Get { index: usize },
    Set { index: usize, val: String },
    Remove { index: usize },
}

#[derive(Subcommand)]
enum StackCmd {
    Push { val: String },
    Pop,
    Top,
    Clear,
    Print,
}

#[derive(Subcommand)]
enum QueueCmd {
    Push { val: String },
    Pop,
    Peek,
    Clear,
    Print,
}

#[derive(Subcommand)]
enum LLCmd {
    AddHead { val: String },
    AddTail { val: String },
    RemoveHead,
    RemoveTail,
    RemoveValue { val: String },
    Print,
    Clear,
}

#[derive(Subcommand)]
enum DLLCmd {
    AddHead { val: String },
    AddTail { val: String },
    RemoveHead,
    RemoveTail,
    RemoveValue { val: String },
    Print,
    Clear,
}

fn main() {
    let cli = Cli::parse();

    match cli.ds {
        DataStructure::Array(cmd) => {
            let mut arr = Array::new(10);
            let _ = arr.read_from_file(ARRAY_FILE);
            match cmd {
                ArrayCmd::Print => {
                    println!("{}", arr.data.join(" "));
                }
                ArrayCmd::Clear => {
                    arr.clear();
                    let _ = arr.write_to_file(ARRAY_FILE);
                    println!("Array cleared.");
                }
                ArrayCmd::AddEnd { val } => {
                    arr.add_end(&val);
                    let _ = arr.write_to_file(ARRAY_FILE);
                    println!("Added {} to end.", val);
                }
                ArrayCmd::Add { index, val } => {
                    if arr.add_at(index, &val) {
                        let _ = arr.write_to_file(ARRAY_FILE);
                        println!("Inserted {} at index {}.", val, index);
                    } else {
                        println!("Index out of range.");
                    }
                }
                ArrayCmd::Get { index } => {
                    match arr.get(index) {
                        Some(v) => println!("Array[{}] = {}", index, v),
                        None => println!("Index out of range."),
                    }
                }
                ArrayCmd::Set { index, val } => {
                    if arr.set(index, &val) {
                        let _ = arr.write_to_file(ARRAY_FILE);
                        println!("Set index {} = {}.", index, val);
                    } else {
                        println!("Index out of range.");
                    }
                }
                ArrayCmd::Remove { index } => {
                    if arr.remove_at(index) {
                        let _ = arr.write_to_file(ARRAY_FILE);
                        println!("Removed element at index {}.", index);
                    } else {
                        println!("Index out of range.");
                    }
                }
            }
        }

        DataStructure::Stack(cmd) => {
            let mut s = Stack::new();
            let _ = s.read_from_file(STACK_FILE);
            match cmd {
                StackCmd::Push { val } => {
                    s.push(val);
                    let _ = s.write_to_file(STACK_FILE);
                }
                StackCmd::Pop => {
                    match s.pop() {
                        Some(v) => println!("Popped: {}", v),
                        None => println!("Stack empty"),
                    }
                    let _ = s.write_to_file(STACK_FILE);
                }
                StackCmd::Top => {
                    match s.top() {
                        Some(v) => println!("Top: {}", v),
                        None => println!("Stack empty"),
                    }
                }
                StackCmd::Clear => {
                    s.clear();
                    let _ = s.write_to_file(STACK_FILE);
                }
                StackCmd::Print => {
                    println!("{}", s.data.join(" "));
                }
            }
        }

        DataStructure::Queue(cmd) => {
            let mut q = Queue::new();
            let _ = q.read_from_file(QUEUE_FILE);
            match cmd {
                QueueCmd::Push { val } => {
                    q.push(val);
                    let _ = q.write_to_file(QUEUE_FILE);
                }
                QueueCmd::Pop => {
                    match q.pop() {
                        Some(v) => println!("Popped: {}", v),
                        None => println!("Queue empty"),
                    }
                    let _ = q.write_to_file(QUEUE_FILE);
                }
                QueueCmd::Peek => {
                    match q.peek() {
                        Some(v) => println!("Front: {}", v),
                        None => println!("Queue empty"),
                    }
                }
                QueueCmd::Clear => {
                    q.clear();
                    let _ = q.write_to_file(QUEUE_FILE);
                }
                QueueCmd::Print => {
                    println!("{}", q.data.join(" "));
                }
            }
        }

        DataStructure::LinkedList(cmd) => {
            let mut list = LinkedList::new();
            let _ = list.read_from_file(LL_FILE);
            match cmd {
                LLCmd::AddHead { val } => list.add_head(val),
                LLCmd::AddTail { val } => list.add_tail(val),
                LLCmd::RemoveHead => { list.remove_head(); },
                LLCmd::RemoveTail => { list.remove_tail(); },
                LLCmd::RemoveValue { val } => { list.remove_value(val); },
                LLCmd::Print => list.print(),
                LLCmd::Clear => list.clear(),
            }
            let _ = list.write_to_file(LL_FILE);
        }

        DataStructure::DLinkedList(cmd) => {
            let mut list = DLinkedList::new();
            let _ = list.read_from_file(DLL_FILE);
            match cmd {
                DLLCmd::AddHead { val } => list.add_head(val),
                DLLCmd::AddTail { val } => list.add_tail(val),
                DLLCmd::RemoveHead => { list.remove_head(); },
                DLLCmd::RemoveTail => { list.remove_tail(); },
                DLLCmd::RemoveValue { val } => { list.remove_value(val); },
                DLLCmd::Print => list.print(),
                DLLCmd::Clear => list.clear(),
            }
            let _ = list.write_to_file(DLL_FILE);
        }
    }
}
