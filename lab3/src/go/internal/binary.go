package internal

import (
	"encoding/binary"
	"fmt"
	"io"
	"os"
)

const (
	ArrayType = iota
	SinglyListType
	DoublyListType
	StackType
	QueueType
	CBTreeType
	HashTableType
)

func WriteString(w io.Writer, s string) error {
	err := binary.Write(w, binary.LittleEndian, int32(len(s)))
	if err != nil {
		return err
	}
	_, err = w.Write([]byte(s))
	return err
}

func ReadString(r io.Reader) (string, error) {
	var length int32
	if err := binary.Read(r, binary.LittleEndian, &length); err != nil {
		return "", err
	}
	if length == 0 {
		return "", nil
	}
	buf := make([]byte, length)
	if _, err := io.ReadFull(r, buf); err != nil {
		return "", err
	}
	return string(buf), nil
}

func (dm *DataManager) SaveBinary(filename string) error {
	file, err := os.Create(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	file.Write([]byte("NSTU"))
	binary.Write(file, binary.LittleEndian, int32(1))

	total := int32(len(dm.Arrays) + len(dm.SinglyLists) + len(dm.DoublyLists) +
		len(dm.Stacks) + len(dm.Queues) + len(dm.Trees) + len(dm.HashTables))
	binary.Write(file, binary.LittleEndian, total)

	for name, obj := range dm.Arrays {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(ArrayType))
		data := obj.ToSlice()
		binary.Write(file, binary.LittleEndian, int32(len(data)))
		for _, v := range data {
			WriteString(file, v)
		}
	}

	for name, obj := range dm.SinglyLists {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(SinglyListType))
		binary.Write(file, binary.LittleEndian, int32(obj.size))
		curr := obj.head
		for curr != nil {
			WriteString(file, curr.Data)
			curr = curr.Next
		}
	}

	for name, obj := range dm.DoublyLists {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(DoublyListType))
		binary.Write(file, binary.LittleEndian, int32(obj.size))
		curr := obj.head
		for curr != nil {
			WriteString(file, curr.Data)
			curr = curr.Next
		}
	}

	for name, obj := range dm.Stacks {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(StackType))
		var temp []string
		curr := obj.list.head
		for curr != nil {
			temp = append([]string{curr.Data}, temp...)
			curr = curr.Next
		}
		binary.Write(file, binary.LittleEndian, int32(len(temp)))
		for _, v := range temp {
			WriteString(file, v)
		}
	}

	for name, obj := range dm.Queues {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(QueueType))
		binary.Write(file, binary.LittleEndian, int32(obj.list.size))
		curr := obj.list.head
		for curr != nil {
			WriteString(file, curr.Data)
			curr = curr.Next
		}
	}

	for name, obj := range dm.Trees {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(CBTreeType))
		data := obj.ToSlice()
		binary.Write(file, binary.LittleEndian, int32(len(data)))
		for _, v := range data {
			WriteString(file, v)
		}
	}

	for name, obj := range dm.HashTables {
		WriteString(file, name)
		binary.Write(file, binary.LittleEndian, int32(HashTableType))
		entries := obj.GetAll()
		binary.Write(file, binary.LittleEndian, int32(len(entries)))
		for _, entry := range entries {
			WriteString(file, entry.Key)
			WriteString(file, entry.Value)
		}
	}

	return nil
}

func (dm *DataManager) LoadBinary(filename string) error {
	file, err := os.Open(filename)
	if err != nil {
		return err
	}
	defer file.Close()

	magic := make([]byte, 4)
	if _, err := io.ReadFull(file, magic); err != nil || string(magic) != "NSTU" {
		return fmt.Errorf("invalid format")
	}

	var version int32
	binary.Read(file, binary.LittleEndian, &version)

	var total int32
	binary.Read(file, binary.LittleEndian, &total)

	for i := 0; i < int(total); i++ {
		name, _ := ReadString(file)
		var typeID int32
		binary.Read(file, binary.LittleEndian, &typeID)

		var size int32
		binary.Read(file, binary.LittleEndian, &size)

		switch typeID {
		case ArrayType:
			arr := NewArray()
			for j := 0; j < int(size); j++ {
				val, _ := ReadString(file)
				arr.Push(val)
			}
			dm.Arrays[name] = arr
		case SinglyListType:
			list := NewSinglyList()
			for j := 0; j < int(size); j++ {
				val, _ := ReadString(file)
				list.PushTail(val)
			}
			dm.SinglyLists[name] = list
		case DoublyListType:
			list := NewDoublyList()
			for j := 0; j < int(size); j++ {
				val, _ := ReadString(file)
				list.PushTail(val)
			}
			dm.DoublyLists[name] = list
		case StackType:
			s := NewStack()
			for j := 0; j < int(size); j++ {
				val, _ := ReadString(file)
				s.Push(val)
			}
			dm.Stacks[name] = s
		case QueueType:
			q := NewQueue()
			for j := 0; j < int(size); j++ {
				val, _ := ReadString(file)
				q.Push(val)
			}
			dm.Queues[name] = q
		case CBTreeType:
			tree := NewCBT()
			for j := 0; j < int(size); j++ {
				val, _ := ReadString(file)
				tree.Insert(val)
			}
			dm.Trees[name] = tree
		case HashTableType:
			ht := NewHashTable(int(size) + 1)
			for j := 0; j < int(size); j++ {
				key, _ := ReadString(file)
				val, _ := ReadString(file)
				ht.Insert(key, val)
			}
			dm.HashTables[name] = ht
		}
	}
	return nil
}