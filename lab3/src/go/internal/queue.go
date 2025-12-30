package internal

type Queue struct {
	list *SinglyList
}

func NewQueue() *Queue {
	return &Queue{
		list: NewSinglyList(),
	}
}

func (q *Queue) Push(value string) {
	q.list.PushTail(value)
}

func (q *Queue) Pop() string {
	if q.list.GetSize() == 0 {
		return ""
	}
	val := q.list.head.Data
	q.list.DelHead()
	return val
}

func (q *Queue) Front() string {
	if q.list.GetSize() == 0 {
		return ""
	}
	return q.list.head.Data
}

func (q *Queue) Size() int {
	return q.list.GetSize()
}

func (q *Queue) Empty() bool {
	return q.list.GetSize() == 0
}

func (q *Queue) Print() string {
	if q.Empty() {
		return "[] (front->back)"
	}
	return q.list.Print() + " (front->back)"
}