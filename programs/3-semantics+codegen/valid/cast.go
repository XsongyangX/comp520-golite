//~65
//~o
//~32
//~111
//~65
//~+6.500000e+001
//~+1.110000e+002
//~+3.230000e+001
//~65
//~111
//~32
package main

func main() {

	// to ascii (string)
	var a int = 65
	println(string(a))

	var r rune = 'o'
	println(string(r))

	// to int
	var k float64 = 32.3
	println(int(k))

	println(int(r))
	println(int(a))

	// to float
	println(float64(a))
	println(float64(r))
	println(float64(k))

	// to rune
	println(rune(a))
	println(rune(r))
	println(rune(k))
}

