/* A switch statement without default and with a non-terminating statement
list is not terminating */
package main

func main(){}

func foo(x int) int {
	switch x {
	case 1: return 10
	case 2:
	}
}