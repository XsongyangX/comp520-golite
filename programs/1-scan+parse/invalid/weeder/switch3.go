/* A switch statement with breaks and without default case is not terminating */
package main

func main(){}

func foo(x float64) float64 {
	switch {
	case x > 0.0: return 1.0
	case x < 0.0: return 0.0
	}
}