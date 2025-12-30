package internal

import (
	"fmt"
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestArray_All(t *testing.T) {
	t.Run("Initialization", func(t *testing.T) {
		arr := NewArray()
		assert.Equal(t, 0, arr.GetSize())
		assert.Equal(t, 4, arr.capacity)
		assert.Equal(t, "[]", arr.Print())
	})

	t.Run("PushAndResize", func(t *testing.T) {
		arr := NewArray()
		for i := 0; i < 5; i++ {
			arr.Push(fmt.Sprintf("val%d", i))
		}
		assert.Equal(t, 5, arr.GetSize())
		assert.Equal(t, 8, arr.capacity)
		
		val, err := arr.Get(4)
		assert.NoError(t, err)
		assert.Equal(t, "val4", val)
	})

	t.Run("InsertOperations", func(t *testing.T) {
		arr := NewArray()
		arr.Push("A")
		arr.Push("C")
		
		err := arr.Insert(1, "B")
		assert.NoError(t, err)
		assert.Equal(t, "[A, B, C]", arr.Print())

		err = arr.Insert(0, "START")
		assert.NoError(t, err)
		assert.Equal(t, "START", arr.data[0])

		assert.Error(t, arr.Insert(-1, "error"))
		assert.Error(t, arr.Insert(10, "error"))
	})

	t.Run("GetAndSet", func(t *testing.T) {
		arr := NewArray()
		arr.Push("initial")

		err := arr.Set(0, "updated")
		assert.NoError(t, err)
		val, _ := arr.Get(0)
		assert.Equal(t, "updated", val)

		_, err = arr.Get(-1)
		assert.Error(t, err)
		_, err = arr.Get(1)
		assert.Error(t, err)

		assert.Error(t, arr.Set(-1, "fail"))
		assert.Error(t, arr.Set(1, "fail"))
	})

	t.Run("RemoveOperations", func(t *testing.T) {
		arr := NewArray()
		arr.Push("1")
		arr.Push("2")
		arr.Push("3")

		ok := arr.Remove(1)
		assert.True(t, ok)
		assert.Equal(t, 2, arr.GetSize())
		assert.Equal(t, "[1, 3]", arr.Print())

		ok = arr.Remove(1)
		assert.True(t, ok)
		assert.Equal(t, "[1]", arr.Print())

		assert.False(t, arr.Remove(-1))
		assert.False(t, arr.Remove(5))
	})

	t.Run("ToSlice", func(t *testing.T) {
		arr := NewArray()
		arr.Push("Go")
		arr.Push("Rust")
		
		slice := arr.ToSlice()
		assert.Len(t, slice, 2)
		assert.Equal(t, "Go", slice[0])
		
		slice[0] = "Modified"
		val, _ := arr.Get(0)
		assert.Equal(t, "Go", val)
	})
}