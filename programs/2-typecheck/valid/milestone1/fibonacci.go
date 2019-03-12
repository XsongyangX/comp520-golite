/* Computes the fibonacci of 10 */
package main

func fib(n int) int {
	if n == 1 {
		return 1
	} else if n == 2 {
		return 1
	} else {
		return fib(n-2) + fib(n-1)
	}
}

func main() {
	// there is no functionality for reading from stdin
	println("Prints fibonacci of 10")
	println(fib(10))
}

