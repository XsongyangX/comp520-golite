//~default
package main

func baz() int {
	return 2
}

func foo() int {
	return 99
}

func main() {
	var a, b int
	c := a + b

	switch foo() {
	case a, baz():
		if b > c {
			break
		}
		println("a baz")
	default:
		println("default")
	}
}

