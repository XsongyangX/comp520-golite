//~appleApple
//~false
//~3
//~+4.000000e+000
//~1024
//~63
//~1
//~7
//~6
//~2
package main

func main() {
	// string concatenation
	var a string = "apple"
	var b string = "Apple"

	println(a + b)
	println(a < b)

	// int division
	var i int
	i = 10 / 3
	println(i)

	// float division
	var j float64
	j = 10.0 / 2.5
	println(j)

	// strange operators

	var l = 1 << 10 // bit shift
	println(l)      // 1024

	var m = 0xFFFF >> 10 // bit shift
	println(m)           // 63

	var bit1 = 4 - 1
	var bit2 = 4 + 1

	println(bit1 & bit2)  // 1
	println(bit1 | bit2)  // 7
	println(bit1 ^ bit2)  // 6
	println(bit1 &^ bit2) // 2

}

