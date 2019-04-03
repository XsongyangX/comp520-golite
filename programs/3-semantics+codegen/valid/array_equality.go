//~true
//~false

package main

func main() {
	var a, b [5] int
	println(a == b)

	b[0] = 1
	println(a == b)

}