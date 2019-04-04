//~true
//~false
//~true
//~false
//~true

package main

func main() {
	var a, b [5] int
	println(a == b)

	b[0] = 1
	println(a == b)
	
	// more complicated cases
	var x,y [7][3]int
	println(x == y)
	
	x[1][1] = 1
	println(x == y)
	
	y[1][1] = 1
	println(x == y)

	var s,t [10][2][3][1][2]string
	println(s == t)

}
