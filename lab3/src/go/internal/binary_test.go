package internal

import (
	"encoding/binary"
	"os"
	"testing"
	"github.com/stretchr/testify/assert"
)

func TestSaveBinary(t *testing.T) {
	tmpFile := "test_storage.bin"
	defer os.Remove(tmpFile)

	dm := NewDataManager()
	dm.Arrays["myArr"] = NewArray()
	dm.Arrays["myArr"].Push("val1")
	dm.HashTables["myTable"] = NewHashTable(4)
	dm.HashTables["myTable"].Insert("key1", "data1")

	err := dm.SaveBinary(tmpFile)
	assert.NoError(t, err)

	file, err := os.Open(tmpFile)
	assert.NoError(t, err)
	defer file.Close()

	magic := make([]byte, 4)
	file.Read(magic)
	assert.Equal(t, "NSTU", string(magic))

	var version int32
	binary.Read(file, binary.LittleEndian, &version)
	assert.Equal(t, int32(1), version)

	var total int32
	binary.Read(file, binary.LittleEndian, &total)
	assert.Equal(t, int32(2), total)
}

func TestBinary_RoundTrip(t *testing.T) {
	tmpFile := "roundtrip.bin"
	defer os.Remove(tmpFile)

	dm1 := NewDataManager()
	dm1.Arrays["a"] = NewArray()
	dm1.Arrays["a"].Push("x")
	dm1.HashTables["h"] = NewHashTable(4)
	dm1.HashTables["h"].Insert("k", "v")

	err := dm1.SaveBinary(tmpFile)
	assert.NoError(t, err)

	dm2 := NewDataManager()
	err = dm2.LoadBinary(tmpFile)
	assert.NoError(t, err)

	assert.Equal(t, dm1.Arrays["a"].Print(), dm2.Arrays["a"].Print())
	v, ok := dm2.HashTables["h"].Get("k")
	assert.True(t, ok)
	assert.Equal(t, "v", v)
}

func TestWriteStringReadString(t *testing.T) {
	tmpFile := "test_str.bin"
	defer os.Remove(tmpFile)

	f, _ := os.Create(tmpFile)
	WriteString(f, "hello")
	f.Close()

	f, _ = os.Open(tmpFile)
	s, err := ReadString(f)
	f.Close()

	assert.NoError(t, err)
	assert.Equal(t, "hello", s)
}

func TestBinary_FullRoundTrip(t *testing.T) {
	tmpFile := "full_test.bin"
	defer os.Remove(tmpFile)

	dm1 := NewDataManager()

	dm1.Arrays["a"] = NewArray()
	dm1.Arrays["a"].Push("1")

	dm1.SinglyLists["sl"] = NewSinglyList()
	dm1.SinglyLists["sl"].PushTail("2")

	dm1.DoublyLists["dl"] = NewDoublyList()
	dm1.DoublyLists["dl"].PushTail("3")

	dm1.Stacks["st"] = NewStack()
	dm1.Stacks["st"].Push("4")

	dm1.Queues["q"] = NewQueue()
	dm1.Queues["q"].Push("5")

	dm1.Trees["t"] = NewCBT()
	dm1.Trees["t"].Insert("6")

	dm1.HashTables["h"] = NewHashTable(4)
	dm1.HashTables["h"].Insert("k", "7")

	err := dm1.SaveBinary(tmpFile)
	assert.NoError(t, err)

	dm2 := NewDataManager()
	err = dm2.LoadBinary(tmpFile)
	assert.NoError(t, err)

	assert.Equal(t, dm1.Arrays["a"].Print(), dm2.Arrays["a"].Print())
	assert.Equal(t, dm1.SinglyLists["sl"].Print(), dm2.SinglyLists["sl"].Print())
	assert.Equal(t, dm1.DoublyLists["dl"].Print(), dm2.DoublyLists["dl"].Print())
	assert.Equal(t, dm1.Stacks["st"].Print(), dm2.Stacks["st"].Print())
	assert.Equal(t, dm1.Queues["q"].Print(), dm2.Queues["q"].Print())
	assert.Equal(t, dm1.Trees["t"].Print(), dm2.Trees["t"].Print())
	
	v, ok := dm2.HashTables["h"].Get("k")
	assert.True(t, ok)
	assert.Equal(t, "7", v)
}