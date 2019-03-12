/* Valid terminating switch */
package main

func main(){}

func foo(x int) int {
	switch x {
	case 1:
		x++
		return x
	case 2:
		x--
		return x
	default:
		x += x
		return x
	}
}