//~30
//~4 7
//~0
//~11
//~3 11 22 33
//~0
//~3
//~0
//~1 0
package main

func main() {
	// array
	var a [30]string
	println(len(a))

	var a_nested [7][4]rune
	println(len(a_nested[0]), len(a_nested))

	// string
	var s string
	println(len(s))
	s = "hello world"
	println(len(s))

	var s_nested [3]string
	s_nested[0] = s
	s_nested[1] = s + s
	s_nested[2] = s + s + s

	println(len(s_nested), len(s_nested[0]), len(s_nested[1]), len(s_nested[2]))

	// slice
	var q []int
	println(len(q))

	q = append(append(append(q, 3), 2), 1)
	println(len(q))

	// slice of slice
	var p [][]rune
	println(len(p))

	var temp []rune
	p = append(p, temp)
	println(len(p), len(p[0]))
}

