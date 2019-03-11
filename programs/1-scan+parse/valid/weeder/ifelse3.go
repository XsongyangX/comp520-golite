/* The if statement is not terminating, but it is not the last statement
of the function. */
package main

func main(){}

func foo(b bool) bool {
	if b {
	} else if !b {
		return false
	} else {
		return true
	}
	return false

}
