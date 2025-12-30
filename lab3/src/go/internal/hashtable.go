package internal

import (
	"fmt"
	"strings"
)

type hashNode struct {
	key   string
	value string
	next  *hashNode
}

type HashEntry struct {
	Key   string
	Value string
}

type HashTable struct {
	buckets  []*hashNode
	capacity int
	count    int
}

const loadFactorThreshold = 0.75

func NewHashTable(initialCapacity int) *HashTable {
	if initialCapacity <= 0 {
		initialCapacity = 16
	}
	return &HashTable{
		buckets:  make([]*hashNode, initialCapacity),
		capacity: initialCapacity,
		count:    0,
	}
}

func (ht *HashTable) hashFunction(key string) int {
	hash := 0
	for i := 0; i < len(key); i++ {
		hash = 31*hash + int(key[i])
	}
	return (hash & 0x7FFFFFFF) % ht.capacity
}

func (ht *HashTable) Insert(key, value string) {
	index := ht.hashFunction(key)
	curr := ht.buckets[index]

	for curr != nil {
		if curr.key == key {
			curr.value = value
			return
		}
		curr = curr.next
	}

	newNode := &hashNode{key: key, value: value, next: ht.buckets[index]}
	ht.buckets[index] = newNode
	ht.count++

	if float64(ht.count)/float64(ht.capacity) > loadFactorThreshold {
		ht.resize()
	}
}

func (ht *HashTable) resize() {
	oldBuckets := ht.buckets
	ht.capacity *= 2
	ht.buckets = make([]*hashNode, ht.capacity)
	ht.count = 0

	for _, node := range oldBuckets {
		for node != nil {
			ht.Insert(node.key, node.value)
			node = node.next
		}
	}
}

func (ht *HashTable) Get(key string) (string, bool) {
	index := ht.hashFunction(key)
	curr := ht.buckets[index]
	for curr != nil {
		if curr.key == key {
			return curr.value, true
		}
		curr = curr.next
	}
	return "", false
}

func (ht *HashTable) Remove(key string) bool {
	index := ht.hashFunction(key)
	curr := ht.buckets[index]
	var prev *hashNode

	for curr != nil {
		if curr.key == key {
			if prev != nil {
				prev.next = curr.next
			} else {
				ht.buckets[index] = curr.next
			}
			ht.count--
			return true
		}
		prev = curr
		curr = curr.next
	}
	return false
}

func (ht *HashTable) GetAll() []HashEntry {
	entries := make([]HashEntry, 0, ht.count)
	for _, node := range ht.buckets {
		for node != nil {
			entries = append(entries, HashEntry{Key: node.key, Value: node.value})
			node = node.next
		}
	}
	return entries
}

func (ht *HashTable) Print() string {
	var sb strings.Builder
	sb.WriteString("{")
	first := true
	for _, node := range ht.buckets {
		for node != nil {
			if !first {
				sb.WriteString(", ")
			}
			fmt.Fprintf(&sb, "%s: %s", node.key, node.value)
			first = false
			node = node.next
		}
	}
	sb.WriteString("}")
	return sb.String()
}

func (ht *HashTable) GetCount() int {
	return ht.count
}