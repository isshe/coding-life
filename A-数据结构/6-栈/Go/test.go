package main

import (
	"fmt"
	"isshe/algo"
)

func main() {
	stack := algo.NewStack()
	stack.Push(123, "isshe")
	fmt.Println(stack.Top())
	fmt.Println(stack.Pop())
	fmt.Println(stack.Top())
	fmt.Println(stack.Size())
	fmt.Println(stack.IsEmpty())
	fmt.Println(stack.Pop())
	fmt.Println(stack.IsEmpty())
}
