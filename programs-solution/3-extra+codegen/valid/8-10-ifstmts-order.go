//~True

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
	if bar(foo(), foo()) == false {
		println("True")
	} else {
		println("False")
	}
}

