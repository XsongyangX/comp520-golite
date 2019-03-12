/* Valid terminating switch */
package main

func main() {}

func foo(r float64) float64 {
	switch {
	case r < 0.0: 
		print(r)
		return r 
	case r > 0.0:
		return r+1.0
	default:
		return 0.0
}