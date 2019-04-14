//~1 2 2

package main

var a int = 0

func foo() int {
	a++
	return a
}

func main() {
	b, c, d := foo(), a, foo()
	println(b, c, d)
}
