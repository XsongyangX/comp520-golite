//~a: len=  2 , cap=  2
//~b: len=  1 , cap=  2

package main

func main() {
	var a, b []int
	a = append(a, 0)
	b = a

	a = append(a, 1)

	// get len and cap
	println("a: len= ", len(a), ", cap= ", cap(a))
	println("b: len= ", len(b), ", cap= ", cap(b))


}

