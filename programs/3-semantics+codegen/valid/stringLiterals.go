//~
//~
//~!"#$%&'()*+,-./
//~0123456789
//~:;<=>?
//~@
//~ABCDEFGHIJKLMNOPQRSTUVWXYZ
//~[\]^_`
//~abcdefghijklmnopqrstuvwxyz
//~{|}~
//~
//~
//~
//~
//~
//~
//~	
//~
//~\'
/* Valid strings */
package main

func main() {
	var s string = ""
	println(s)
	s = " "
	println(s)
	s = "!\"#$%&'()*+,-./"
	println(s)
	s = "0123456789"
	println(s)
	s = ":;<=>?"
	println(s)
	s = "@"
	println(s)
	s = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
	println(s)
	s = "[\\]^_`"
	println(s)
	s = "abcdefghijklmnopqrstuvwxyz"
	println(s)
	s = "{|}~"
	println(s)

	s = "\a"
	println(s)
	s = "\b"
	println(s)
	s = "\f"
	println(s)
	s = "\n"
	println(s) 
	s = "\r"
	println(s) 
	s = "\t"
	println(s) 
	s = "\v"
	println(s)
	s = "\\'"
	println(s)
}
