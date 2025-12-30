package internal

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestHashTable_InsertAndGet(t *testing.T) {
	ht := NewHashTable(4)
	ht.Insert("user1", "admin")
	ht.Insert("user2", "guest")

	val, ok := ht.Get("user1")
	assert.True(t, ok)
	assert.Equal(t, "admin", val)

	val, ok = ht.Get("user2")
	assert.True(t, ok)
	assert.Equal(t, "guest", val)

	val, ok = ht.Get("unknown")
	assert.False(t, ok)
	assert.Equal(t, "", val)
}

func TestHashTable_Update(t *testing.T) {
	ht := NewHashTable(16)
	ht.Insert("key", "old")
	ht.Insert("key", "new")

	val, _ := ht.Get("key")
	assert.Equal(t, "new", val)
	assert.Equal(t, 1, ht.GetCount())
}

func TestHashTable_Remove(t *testing.T) {
	ht := NewHashTable(16)
	ht.Insert("A", "1")
	ht.Insert("B", "2")

	assert.True(t, ht.Remove("A"))
	assert.False(t, ht.Remove("A"))
	
	_, ok := ht.Get("A")
	assert.False(t, ok)
	assert.Equal(t, 1, ht.GetCount())
}

func TestHashTable_Resize(t *testing.T) {
	ht := NewHashTable(2)
	ht.Insert("1", "a")
	ht.Insert("2", "b")
	ht.Insert("3", "c") // Здесь должен сработать resize (3/2 > 0.75)

	assert.Equal(t, 3, ht.GetCount())
	assert.True(t, ht.capacity >= 4)
	
	val, ok := ht.Get("1")
	assert.True(t, ok)
	assert.Equal(t, "a", val)
}

func TestHashTable_Print(t *testing.T) {
	ht := NewHashTable(10)
	ht.Insert("k", "v")
	assert.Equal(t, "{k: v}", ht.Print())
}