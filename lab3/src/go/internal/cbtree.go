package internal

import (
	"strings"
)

type CBTNode struct {
	Data  string
	Left  *CBTNode
	Right *CBTNode
}

type CompleteBinaryTree struct {
	Root      *CBTNode
	nodeCount int
}

func NewCBT() *CompleteBinaryTree {
	return &CompleteBinaryTree{}
}

func (t *CompleteBinaryTree) Insert(value string) {
	newNode := &CBTNode{Data: value}
	if t.Root == nil {
		t.Root = newNode
		t.nodeCount = 1
		return
	}

	queue := []*CBTNode{t.Root}
	for len(queue) > 0 {
		temp := queue[0]
		queue = queue[1:]

		if temp.Left == nil {
			temp.Left = newNode
			break
		} else {
			queue = append(queue, temp.Left)
		}

		if temp.Right == nil {
			temp.Right = newNode
			break
		} else {
			queue = append(queue, temp.Right)
		}
	}
	t.nodeCount++
}

func (t *CompleteBinaryTree) Search(value string) bool {
	if t.Root == nil {
		return false
	}
	queue := []*CBTNode{t.Root}
	for len(queue) > 0 {
		temp := queue[0]
		queue = queue[1:]

		if temp.Data == value {
			return true
		}
		if temp.Left != nil {
			queue = append(queue, temp.Left)
		}
		if temp.Right != nil {
			queue = append(queue, temp.Right)
		}
	}
	return false
}

func (t *CompleteBinaryTree) ToSlice() []string {
	var res []string
	if t.Root == nil {
		return res
	}
	queue := []*CBTNode{t.Root}
	for len(queue) > 0 {
		temp := queue[0]
		queue = queue[1:]
		res = append(res, temp.Data)
		if temp.Left != nil {
			queue = append(queue, temp.Left)
		}
		if temp.Right != nil {
			queue = append(queue, temp.Right)
		}
	}
	return res
}

func (t *CompleteBinaryTree) Print() string {
	if t.Root == nil {
		return "Empty Tree"
	}
	var sb strings.Builder
	t.printHelper(t.Root, "", false, &sb)
	return sb.String()
}

func (t *CompleteBinaryTree) printHelper(node *CBTNode, prefix string, isLeft bool, sb *strings.Builder) {
	if node == nil {
		return
	}
	marker := "└── "
	if isLeft {
		marker = "├── "
	}
	sb.WriteString(prefix + marker + node.Data + "\n")

	newPrefix := prefix
	if isLeft {
		newPrefix += "│   "
	} else {
		newPrefix += "    "
	}

	if node.Left != nil || node.Right != nil {
		t.printHelper(node.Left, newPrefix, true, sb)
		t.printHelper(node.Right, newPrefix, false, sb)
	}
}

func (t *CompleteBinaryTree) GetSize() int {
	return t.nodeCount
}