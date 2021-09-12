package Go

type Stack struct {
	Elements []interface{}
}

func NewStack() *Stack {
	return &Stack{}
}

func (stack *Stack) Top() interface{} {
	if len(stack.Elements) > 0 {
		return stack.Elements[len(stack.Elements)-1]
	}
	return nil
}

func (stack *Stack) Pop() interface{} {
	elem := stack.Top()
	if elem != nil {
		stack.Elements = stack.Elements[:len(stack.Elements)-1]
	}
	return elem
}

func (stack *Stack) Push(pushElems ...interface{}) {
	stack.Elements = append(stack.Elements, pushElems...)
}

func (stack *Stack) Size() int {
	return len(stack.Elements)
}

func (stack *Stack) IsEmpty() bool {
	return stack.Elements == nil || stack.Size() == 0
}
