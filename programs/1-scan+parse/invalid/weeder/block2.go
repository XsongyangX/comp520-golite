/* A non-void function that ends in a non-terminating block. */
package main

func main(){}

func foo(x int) rune {
	{
		x++
	}
}
