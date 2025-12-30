package internal

import (
	"strings"
)

type SNode struct {
	Data string
	Next *SNode
}

type SinglyList struct {
	head *SNode
	tail *SNode
	size int
}

func NewSinglyList() *SinglyList {
	return &SinglyList{}
}

func (l *SinglyList) PushHead(value string) {
	newNode := &SNode{Data: value}
	if l.head == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		newNode.Next = l.head
		l.head = newNode
	}
	l.size++
}

func (l *SinglyList) PushTail(value string) {
	newNode := &SNode{Data: value}
	if l.tail == nil {
		l.head = newNode
		l.tail = newNode
	} else {
		l.tail.Next = newNode
		l.tail = newNode
	}
	l.size++
}

func (l *SinglyList) DelHead() bool {
	if l.head == nil {
		return false
	}
	l.head = l.head.Next
	if l.head == nil {
		l.tail = nil
	}
	l.size--
	return true
}

func (l *SinglyList) DelTail() bool {
	if l.head == nil {
		return false
	}
	if l.head == l.tail {
		return l.DelHead()
	}
	curr := l.head
	for curr.Next != l.tail {
		curr = curr.Next
	}
	l.tail = curr
	l.tail.Next = nil
	l.size--
	return true
}

func (l *SinglyList) Find(value string) bool {
	curr := l.head
	for curr != nil {
		if curr.Data == value {
			return true
		}
		curr = curr.Next
	}
	return false
}

func (l *SinglyList) Print() string {
	if l.head == nil {
		return "[]"
	}
	var sb strings.Builder
	sb.WriteString("[")
	curr := l.head
	for curr != nil {
		sb.WriteString(curr.Data)
		if curr.Next != nil {
			sb.WriteString(" -> ")
		}
		curr = curr.Next
	}
	sb.WriteString("]")
	return sb.String()
}

func (l *SinglyList) GetSize() int {
	return l.size
}