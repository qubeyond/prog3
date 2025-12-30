package internal

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestDoublyList_Push(t *testing.T) {
	l := NewDoublyList()
	l.PushTail("B")
	l.PushHead("A")
	l.PushTail("C")

	assert.Equal(t, 3, l.GetSize())
	assert.Equal(t, "[A <-> B <-> C]", l.Print())
}

func TestDoublyList_Delete(t *testing.T) {
	l := NewDoublyList()
	l.PushTail("10")
	l.PushTail("20")
	l.PushTail("30")

	l.DelHead()
	assert.Equal(t, "[20 <-> 30]", l.Print())

	l.DelTail()
	assert.Equal(t, "[20]", l.Print())

	l.DelTail()
	assert.Equal(t, "[]", l.Print())
	assert.Equal(t, 0, l.GetSize())
}

func TestDoublyList_GetSet(t *testing.T) {
	l := NewDoublyList()
	l.PushTail("one")
	l.PushTail("two")
	l.PushTail("three")

	assert.Equal(t, "two", l.Get(1))
	assert.Equal(t, "three", l.Get(2))

	err := l.Set(1, "updated")
	assert.NoError(t, err)
	assert.Equal(t, "updated", l.Get(1))

	err = l.Set(10, "fail")
	assert.Error(t, err)
}

func TestDoublyList_Empty(t *testing.T) {
	l := NewDoublyList()
	assert.False(t, l.DelHead())
	assert.False(t, l.DelTail())
	assert.Equal(t, "", l.Get(0))
}