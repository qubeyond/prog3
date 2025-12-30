package internal

import (
	"errors"
	"strings"
)

type DNode struct {
	Data string
	Next *DNode
	Prev *DNode
}

type DoublyList struct {
	head *DNode
	tail *DNode
	size int
}

func NewDoublyList() *DoublyList {
	return &DoublyList{}
}

func (l *DoublyList) PushHead(value string) {
	newNode := &DNode{Data: value}
	if l.head == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		newNode.Next = l.head
		l.head.Prev = newNode
		l.head = newNode
	}
	l.size++
}

func (l *DoublyList) PushTail(value string) {
	newNode := &DNode{Data: value}
	if l.tail == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		newNode.Prev = l.tail
		l.tail.Next = newNode
		l.tail = newNode
	}
	l.size++
}

func (l *DoublyList) DelHead() bool {
	if l.head == nil {
		return false
	}
	l.head = l.head.Next
	if l.head != nil {
		l.head.Prev = nil
	} else {
		l.tail = nil
	}
	l.size--
	return true
}

func (l *DoublyList) DelTail() bool {
	if l.tail == nil {
		return false
	}
	l.tail = l.tail.Prev
	if l.tail != nil {
		l.tail.Next = nil
	} else {
		l.head = nil
	}
	l.size--
	return true
}

func (l *DoublyList) Get(index int) string {
	if index < 0 || index >= l.size {
		return ""
	}
	var curr *DNode
	if index > l.size/2 {
		curr = l.tail
		for i := l.size - 1; i > index; i-- {
			curr = curr.Prev
		}
	} else {
		curr = l.head
		for i := 0; i < index; i++ {
			curr = curr.Next
		}
	}
	return curr.Data
}

func (l *DoublyList) Set(index int, value string) error {
	if index < 0 || index >= l.size {
		return errors.New("index out of bounds")
	}
	var curr *DNode
	if index > l.size/2 {
		curr = l.tail
		for i := l.size - 1; i > index; i-- {
			curr = curr.Prev
		}
	} else {
		curr = l.head
		for i := 0; i < index; i++ {
			curr = curr.Next
		}
	}
	curr.Data = value
	return nil
}

func (l *DoublyList) Print() string {
	if l.head == nil {
		return "[]"
	}
	var sb strings.Builder
	sb.WriteString("[")
	curr := l.head
	for curr != nil {
		sb.WriteString(curr.Data)
		if curr.Next != nil {
			sb.WriteString(" <-> ")
		}
		curr = curr.Next
	}
	sb.WriteString("]")
	return sb.String()
}

func (l *DoublyList) GetSize() int {
	return l.size
}