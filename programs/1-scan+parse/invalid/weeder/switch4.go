/* A switch statement with breaks but non-terminating statement lists 
is not terminating */
package main

func main(){}

func foo(x float64) float64 {
	switch {
	case x > 0.0: 
	case x < 0.0: return 0.0
	default:
		break
		return 1.0
	}
}