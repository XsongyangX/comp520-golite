//~30
//~4 7
//~0
//~4
//~0
//~2 0
package main

func main() {
	// array
	var a [30]string
	println(cap(a))

	var a_nested [7][4]rune
	println(cap(a_nested[0]), cap(a_nested))

	// slice
	var q []int
	println(cap(q))

	q = append(append(append(q, 3), 2), 1)
	println(cap(q))

	// slice of slice
	var p [][]rune
	println(cap(p))

	var temp []rune
	p = append(p, temp)
	println(cap(p), cap(p[0]))
}

