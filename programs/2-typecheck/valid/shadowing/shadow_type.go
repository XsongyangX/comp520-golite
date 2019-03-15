/* Valid type shadowing */
package main

type x int

func main(){
	type x float64

	for {
		type x struct {
			j,k int
		}
	}
}