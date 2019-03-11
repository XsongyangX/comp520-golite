/* Missing return statement in if-else_if-else_if-else */
package main

func main(){}

func fib(n int) int {
	if n > 100 {
		return 100
	} else if n > 10 {
	} else if n > 1 {
		return 1
	} else {
		return 0
	}
}
