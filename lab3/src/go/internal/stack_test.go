package internal

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestStack_Operations(t *testing.T) {
	s := NewStack()
	assert.True(t, s.Empty())

	s.Push("низ")
	s.Push("середина")
	s.Push("верх")

	assert.Equal(t, 3, s.Size())
	assert.Equal(t, "верх", s.Top())

	assert.Equal(t, "верх", s.Pop())
	assert.Equal(t, "середина", s.Pop())
	assert.Equal(t, 1, s.Size())
	assert.Equal(t, "низ", s.Top())

	assert.Equal(t, "низ", s.Pop())
	assert.True(t, s.Empty())
}

func TestStack_Empty(t *testing.T) {
	s := NewStack()
	assert.Equal(t, "", s.Pop())
	assert.Equal(t, "", s.Top())
	assert.Equal(t, "[] (top->bottom)", s.Print())
}

func TestStack_Print(t *testing.T) {
	s := NewStack()
	s.Push("первый")
	s.Push("второй")
	// Стек вставляет в голову списка, поэтому порядок будет такой:
	assert.Equal(t, "[второй -> первый] (top->bottom)", s.Print())
}