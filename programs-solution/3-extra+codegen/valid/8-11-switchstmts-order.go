//~1
//~2
//~3
//~4
//~case

package main

var glob int

func foo() int {
	glob++
	return glob
}

func bar(a int, b int) int {
	println(a)
	println(b)

	return a - b
}

func main() {
	switch bar(foo(), foo()) {
	case bar(foo(), foo()), bar(foo(), foo()):
		println("case")
	default:
		println("default")
	}
}

