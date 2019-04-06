//~3
//~3
//~9
//~3
//~3
//~3
//~3
package main

func main() {

	type num int
	var i num = num(3)
	println(i)
	println(int(i))
	println(num(9))

	type count num
	var k count = count(i)
	println(k)
	println(int(k))
	println(num(k))
	println(count(k))
}

