/* A switch statement with a non-terminating body somewhere is 
not terminating */
package main

func main(){}

func foo(x float64) float64 {
	switch {
	case x > 0.0: return 3.0
	case x < 0.0: return 3.0
	default: 
		x = x
	}
}