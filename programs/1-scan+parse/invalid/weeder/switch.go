/* A switch statement with non-terminating bodies is not terminating */
package main

func main(){}

func foo(x float64) float64 {
	switch {
	case x > 0.0:
	case x < 0.0:
	default: 
	}
}