//~1
//~1

package main

func main() {
	var a, b []int
	a = append(a, 0)
	b = a

	a = append(a, 13)
	a[0] = 1

	println(a[0])
	println(b[0])

	_ = b
}

