//~13
//~0

package main

func main() {
	var a, b []int
	a = append(a, 0)
	a = append(a, 1)
	b = a

	a = append(a, 2)
	a[0] = 13

	println(a[0])
	println(b[0])

}

