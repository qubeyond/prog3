package internal

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestQueue_Operations(t *testing.T) {
	q := NewQueue()
	assert.True(t, q.Empty())

	q.Push("один")
	q.Push("два")
	q.Push("три")

	assert.Equal(t, 3, q.Size())
	assert.Equal(t, "один", q.Front())

	assert.Equal(t, "один", q.Pop())
	assert.Equal(t, "два", q.Pop())
	assert.Equal(t, 1, q.Size())
	assert.Equal(t, "три", q.Front())

	assert.Equal(t, "три", q.Pop())
	assert.True(t, q.Empty())
}

func TestQueue_Empty(t *testing.T) {
	q := NewQueue()
	assert.Equal(t, "", q.Pop())
	assert.Equal(t, "", q.Front())
	assert.Equal(t, "[] (front->back)", q.Print())
}

func TestQueue_Print(t *testing.T) {
	q := NewQueue()
	q.Push("A")
	q.Push("B")
	assert.Equal(t, "[A -> B] (front->back)", q.Print())
}