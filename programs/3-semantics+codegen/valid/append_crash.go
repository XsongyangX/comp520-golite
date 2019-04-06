//~Access out of range.

package main

func main() {
	var a, b []int
	a = append(a, 0)
	b = a

	a = append(a, 1)

	_ = b[1]
}

