//~0
//~0
//~
//~0

package main

type num int

func main(){
	var n num
	println(n) // 0

	type integer num
	var i integer
	println(i) // 0

	type time struct {
		day string
		hour int
	}
	var t time
	println(t.day) // ""
	println(t.hour) // 0
}
