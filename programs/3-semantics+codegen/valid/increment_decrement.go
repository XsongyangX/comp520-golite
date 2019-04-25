//~1
//~+3.000000e+000
//~98
//~-2
//~-1.353000e+001
//~62
//~in foo
//~1
package main

func foo() int {
	println("in foo")
	return 0
}

func main() {

	// increment
	n := 0
	n++
	println(n)

	x := 2.0
	x++
	println(x)

	r := 'a'
	r++
	println(r)

	// decrement
	m := -1
	m--
	println(m)

	y := -12.530
	y--
	println(y)

	s := '?'
	s--
	println(s)

	// advanced
	var a [1]int
	a[foo()]++ // call foo once -> "in foo", a[0] = 1
	println(a[0])



}

