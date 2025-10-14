use rust::a::Array;

#[test]
fn test_init() {
    let arr = Array::new(10);
    assert_eq!(arr.len(), 0);
}

#[test]
fn test_add_end() {
    let mut arr = Array::new(10);
    arr.add_end("A");
    arr.add_end("B");
    assert_eq!(arr.data, vec!["A", "B"]);
}

#[test]
fn test_add_at() {
    let mut arr = Array::new(10);
    arr.add_end("A");
    arr.add_end("C");
    assert!(arr.add_at(1, "B"));
    assert_eq!(arr.data, vec!["A", "B", "C"]);
    assert!(!arr.add_at(10, "Z"));
}

#[test]
fn test_get_set() {
    let mut arr = Array::new(10);
    arr.add_end("X");
    arr.add_end("Y");
    assert_eq!(arr.get(0).unwrap(), "X");
    assert_eq!(arr.get(1).unwrap(), "Y");
    assert!(arr.set(0, "Z"));
    assert_eq!(arr.get(0).unwrap(), "Z");
    assert!(!arr.set(5, "Q"));
}

#[test]
fn test_remove() {
    let mut arr = Array::new(10);
    arr.add_end("A");
    arr.add_end("B");
    arr.add_end("C");
    assert!(arr.remove_at(1));
    assert_eq!(arr.data, vec!["A", "C"]);
    assert!(!arr.remove_at(5));
}

#[test]
fn test_file_io() {
    let mut arr = Array::new(10);
    arr.add_end("Hello");
    arr.add_end("World");
    arr.write_to_file("test_array.txt").unwrap();
    let mut arr2 = Array::new(10);
    arr2.read_from_file("test_array.txt").unwrap();
    assert_eq!(arr2.data, vec!["Hello", "World"]);
    std::fs::remove_file("test_array.txt").unwrap();
}
