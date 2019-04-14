//~1
//~2
//~3

package main

var glob int

func foo(a int, b int) int {
	println(a)
	println(b)
	return a + b
}

func bar() int {
	glob++
	return glob
}

func main() {
	var a int = foo(bar(), bar())
	println(a)
}
