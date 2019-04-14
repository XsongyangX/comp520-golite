//~1 1 1
//~2 3 4 5
//~0
//~0
//~1
//~0 1
//~1
//~2
//~2

package main

var glob int
var array [6]int

func foo() int {
	glob++
	return glob
}

func bar(a int) int {
	println(a)
	return a
}

func baz() [6]int {
	glob++
	return array
}

func main() {
	array[0] = 0
	array[1] = 1
	array[2] = 2
	array[3] = 3
	array[4] = 4
	array[5] = 5

	println(glob, foo(), glob)
	println(foo(), array[foo()], array[foo()], foo())
	println(bar(bar(0)), bar(1))

	glob = 0
	println(baz()[glob])
	println(bar(baz()[glob]))
}
