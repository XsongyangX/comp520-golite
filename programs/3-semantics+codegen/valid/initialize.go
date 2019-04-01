//~false
//~0
//~+0.000000e+00
//~
//~0
//~false
//~0
//~
//~0
package main

func main() {
	
	// initialization of base types
	var b bool // false
	println(b)

	var n int // 0
	println(n)

	var x float64
	println(x) // +0.000000e+00

	var s string
	println(s) // "" (when printed, it shows nothing)

	var r rune
	println(r) // 0
	
	// structs
	var xx struct { m bool; }
	println(xx.m) // false

	var ooo struct{
		x int
		s struct {
			k string
			l rune
		}
	}
	println(ooo.x) // 0
	println(ooo.s.k) // ""
	println(ooo.s.l) // 0

}
