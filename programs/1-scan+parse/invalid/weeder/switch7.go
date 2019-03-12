/* A switch statement in which breaks are present, the default case
is absent and there is a non-terminating statement list. */
package main

func main(){}

func foo(x int) int {
	switch x {
	case 1: break
	case 2: return 3
	}
}