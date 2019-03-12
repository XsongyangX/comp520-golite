/* A function that ends in a non-terminating block*/
package main

func main(){}

func foo() string {
	{
	}
}
