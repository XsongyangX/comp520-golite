//~End

package main

var glob int

func foo() int {
	glob++
	return glob
}

func bar(a int, b int) bool {
	return a > b
}

func main() {
	for bar(foo(), foo()) {
		println("Loop")
		break
	}
	println("End")
}

