//~11
//~2

package main

var glob int

var a [5]int

func foo(a int, b int) {
	println(a)
	println(b)
}

func bar() int {
	glob++
	return glob
}

func baz(b int) int {
	a[0] = 10
	a[1] = 11
	a[2] = 12
	a[3] = 13
	a[4] = 14
	return b
}

func main() {
	glob = 0

	a[0] = 0
	a[1] = 1
	a[2] = 2
	a[3] = 3
	a[4] = 4

	foo(a[bar()], baz(bar()))
}
