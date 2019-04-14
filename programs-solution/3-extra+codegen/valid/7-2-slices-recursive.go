//~Success

package main

func main() {
	type A struct {
		a []A
	}
	var a A

	type B [3][]B
	var b B

	type C []C
	var c, d C
	c = append(c, d)

	type D [][]D
	var e, f D

	println("Success")
}

