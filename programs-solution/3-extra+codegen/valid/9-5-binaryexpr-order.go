//~0
//~4 5
//~true
//~6 7
//~8 9
//~true

package main

var glob int

func bar(a int, b int) bool {
	println(a, b)
	return a < b
}

func foo() int {
	glob++
	return glob
}

func main() {
	glob = 0
	var a int = foo() + foo() - foo()
	println(a)

	var b bool = bar(foo(), foo()) || bar(foo(), foo())
	println(b)

	var c bool = bar(foo(), foo()) && bar(foo(), foo())
	println(c)
}
