/* A non-void function ends in a non-terminating block */
package main

func main(){}

func foo(n int) string {
	{
		
	}
	{
		return "hi"
		{}
	}
}