package internal

import (
	"errors"
	"strings"
)

type Array struct {
	data     []string
	size     int
	capacity int
}

func NewArray() *Array {
	return &Array{
		data:     make([]string, 4),
		size:     0,
		capacity: 4,
	}
}

func (a *Array) resize(newCapacity int) {
	newData := make([]string, newCapacity)
	copy(newData, a.data[:a.size])
	a.data = newData
	a.capacity = newCapacity
}

func (a *Array) Push(value string) {
	if a.size >= a.capacity {
		a.resize(a.capacity * 2)
	}
	a.data[a.size] = value
	a.size++
}

func (a *Array) Insert(index int, value string) error {
	if index < 0 || index > a.size {
		return errors.New("index out of bounds")
	}
	if a.size >= a.capacity {
		a.resize(a.capacity * 2)
	}
	copy(a.data[index+1:a.size+1], a.data[index:a.size])
	a.data[index] = value
	a.size++
	return nil
}

func (a *Array) Get(index int) (string, error) {
	if index < 0 || index >= a.size {
		return "", errors.New("index out of bounds")
	}
	return a.data[index], nil
}

func (a *Array) Set(index int, value string) error {
	if index < 0 || index >= a.size {
		return errors.New("index out of bounds")
	}
	a.data[index] = value
	return nil
}

func (a *Array) Remove(index int) bool {
	if index < 0 || index >= a.size {
		return false
	}
	copy(a.data[index:a.size-1], a.data[index+1:a.size])
	a.size--
	return true
}

func (a *Array) ToSlice() []string {
	res := make([]string, a.size)
	copy(res, a.data[:a.size])
	return res
}

func (a *Array) Print() string {
	var sb strings.Builder
	sb.WriteString("[")
	for i := 0; i < a.size; i++ {
		sb.WriteString(a.data[i])
		if i < a.size-1 {
			sb.WriteString(", ")
		}
	}
	sb.WriteString("]")
	return sb.String()
}

func (a *Array) GetSize() int { return a.size }