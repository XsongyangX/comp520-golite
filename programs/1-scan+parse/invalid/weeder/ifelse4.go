/* Missing return statement in if-else_if-else */
package main

func main(){}

func fib(n int) int {
	if n > 100{
	} else if n > 10 {
		return 0
	} else {
		return 1
	}
}
