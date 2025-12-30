package internal

import (
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestCBT_InsertAndSearch(t *testing.T) {
	tree := NewCBT()
	tree.Insert("Root")
	tree.Insert("L1")
	tree.Insert("R1")

	assert.Equal(t, 3, tree.GetSize())
	assert.True(t, tree.Search("Root"))
	assert.True(t, tree.Search("L1"))
	assert.True(t, tree.Search("R1"))
	assert.False(t, tree.Search("Unknown"))
}

func TestCBT_ToSlice(t *testing.T) {
	tree := NewCBT()
	tree.Insert("1")
	tree.Insert("2")
	tree.Insert("3")
	tree.Insert("4")

	expected := []string{"1", "2", "3", "4"}
	assert.Equal(t, expected, tree.ToSlice())
}

func TestCBT_Print(t *testing.T) {
    tree := NewCBT()
    assert.Equal(t, "Empty Tree", tree.Print())

    tree.Insert("A")
    tree.Insert("B")
    
    output := tree.Print()
    assert.Contains(t, output, "A")
    assert.Contains(t, output, "├── B") 
}

func TestCBT_Empty(t *testing.T) {
	tree := NewCBT()
	assert.Equal(t, 0, tree.GetSize())
	assert.False(t, tree.Search("любой"))
	assert.Empty(t, tree.ToSlice())
}