/* A non-void function that does not end in a terminating block */
package main

func main(){}

func foo() rune {
	{
		if true{
			return 'e'
		}
	}
}
