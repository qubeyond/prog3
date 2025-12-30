package internal

import (
	"strconv"
	"strings"
)

type DataManager struct {
	Arrays      map[string]*Array
	SinglyLists map[string]*SinglyList
	DoublyLists map[string]*DoublyList
	Stacks      map[string]*Stack
	Queues      map[string]*Queue
	Trees       map[string]*CompleteBinaryTree
	HashTables  map[string]*HashTable
}

func NewDataManager() *DataManager {
	return &DataManager{
		Arrays:      make(map[string]*Array),
		SinglyLists: make(map[string]*SinglyList),
		DoublyLists: make(map[string]*DoublyList),
		Stacks:      make(map[string]*Stack),
		Queues:      make(map[string]*Queue),
		Trees:       make(map[string]*CompleteBinaryTree),
		HashTables:  make(map[string]*HashTable),
	}
}

func (dm *DataManager) Execute(query string) string {
	parts := strings.Fields(query)
	if len(parts) < 2 {
		return "Error: Invalid command"
	}

	cmd := parts[0]
	name := parts[1]

	switch {
	case strings.HasPrefix(cmd, "M"):
		if _, ok := dm.Arrays[name]; !ok { dm.Arrays[name] = NewArray() }
		arr := dm.Arrays[name]
		if cmd == "MPUSH" && len(parts) > 2 {
			arr.Push(parts[2])
			return "OK"
		}
		if cmd == "MGET" && len(parts) > 2 {
			idx, _ := strconv.Atoi(parts[2])
			val, err := arr.Get(idx)
			if err != nil { return err.Error() }
			return val
		}

	case strings.HasPrefix(cmd, "S"):
		if _, ok := dm.Stacks[name]; !ok { dm.Stacks[name] = NewStack() }
		s := dm.Stacks[name]
		if cmd == "SPUSH" && len(parts) > 2 {
			s.Push(parts[2])
			return "OK"
		}
		if cmd == "SPOP" { return s.Pop() }

	case strings.HasPrefix(cmd, "Q"):
		if _, ok := dm.Queues[name]; !ok { dm.Queues[name] = NewQueue() }
		q := dm.Queues[name]
		if cmd == "QPUSH" && len(parts) > 2 {
			q.Push(parts[2])
			return "OK"
		}
		if cmd == "QPOP" { return q.Pop() }

	case strings.HasPrefix(cmd, "H"):
		if _, ok := dm.HashTables[name]; !ok { dm.HashTables[name] = NewHashTable(16) }
		ht := dm.HashTables[name]
		if cmd == "HSET" && len(parts) > 3 {
			ht.Insert(parts[2], parts[3])
			return "OK"
		}
		if cmd == "HGET" && len(parts) > 2 {
			val, ok := ht.Get(parts[2])
			if !ok { return "Not found" }
			return val
		}

	case cmd == "PRINT":
		if v, ok := dm.Arrays[name]; ok { return v.Print() }
		if v, ok := dm.HashTables[name]; ok { return v.Print() }
		if v, ok := dm.Stacks[name]; ok { return v.Print() }
		if v, ok := dm.Queues[name]; ok { return v.Print() }
		if v, ok := dm.SinglyLists[name]; ok { return v.Print() }
		if v, ok := dm.DoublyLists[name]; ok { return v.Print() }
		if v, ok := dm.Trees[name]; ok { return v.Print() }
	}

	return "Error: Unknown command"
}