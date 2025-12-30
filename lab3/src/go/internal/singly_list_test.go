package internal

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestSinglyList_Push(t *testing.T) {
	l := NewSinglyList()
	l.PushTail("B")
	l.PushHead("A")
	l.PushTail("C")

	assert.Equal(t, 3, l.GetSize())
	assert.Equal(t, "[A -> B -> C]", l.Print())
}

func TestSinglyList_Delete(t *testing.T) {
	l := NewSinglyList()
	l.PushTail("1")
	l.PushTail("2")
	l.PushTail("3")

	l.DelHead()
	assert.Equal(t, "[2 -> 3]", l.Print())
	
	l.DelTail()
	assert.Equal(t, "[2]", l.Print())
	
	l.DelHead()
	assert.Equal(t, "[]", l.Print())
	assert.Equal(t, 0, l.GetSize())
}

func TestSinglyList_Find(t *testing.T) {
	l := NewSinglyList()
	l.PushTail("find_me")
	
	assert.True(t, l.Find("find_me"))
	assert.False(t, l.Find("not_here"))
}

func TestSinglyList_EmptyOperations(t *testing.T) {
	l := NewSinglyList()
	assert.False(t, l.DelHead())
	assert.False(t, l.DelTail())
	assert.Equal(t, "[]", l.Print())
}