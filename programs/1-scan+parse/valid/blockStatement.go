/* A valid standalone block statement */
package main

func main() {
	var x int
	{
		x++
		y := x
	}
	{
		print(x)
	}
	{
	}
	{
		{
		}
	}
}
