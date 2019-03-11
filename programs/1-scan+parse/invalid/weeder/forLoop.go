/* An infinite for-loop with a break inside is not a terminating statement */
package main

func main(){}

func foo(x float64) float64 {
	for{
		break
	}
}
