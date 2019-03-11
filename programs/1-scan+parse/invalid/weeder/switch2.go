/* A switch statement with breaks is not terminating */
package main

func main(){}

func foo(x float64) float64 {
	switch x {
	case x > 0: return 1.0
	case x < 0: return 0.0
	default:
		break
		return 10.0
	}
}