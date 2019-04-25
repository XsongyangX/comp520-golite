//~9000
//~9000
//~9000
//~9000

package main

func main() {
	var a int = 9000
	println(a)
	{
		var b int = a
		_ = b
		println(a)
		var a int = a
		println(a)
	}
	println(a)
}
