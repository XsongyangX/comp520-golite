/* A for-loop that has a break statement inside, but it is not the last 
statement of the function. */
package main

func main(){}

func foo(x string) string {
	for {
		break
	}
	return "hi"
}
