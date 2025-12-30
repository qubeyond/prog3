package internal

type Stack struct {
	list *SinglyList
}

func NewStack() *Stack {
	return &Stack{
		list: NewSinglyList(),
	}
}

func (s *Stack) Push(value string) {
	s.list.PushHead(value)
}

func (s *Stack) Pop() string {
	if s.list.GetSize() == 0 {
		return ""
	}
	val := s.list.head.Data
	s.list.DelHead()
	return val
}

func (s *Stack) Top() string {
	if s.list.GetSize() == 0 {
		return ""
	}
	return s.list.head.Data
}

func (s *Stack) Size() int {
	return s.list.GetSize()
}

func (s *Stack) Empty() bool {
	return s.list.GetSize() == 0
}

func (s *Stack) Print() string {
	if s.Empty() {
		return "[] (top->bottom)"
	}
	return s.list.Print() + " (top->bottom)"
}