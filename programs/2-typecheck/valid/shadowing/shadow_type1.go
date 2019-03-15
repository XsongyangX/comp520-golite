/* Nested shadow typing */
package main

type num int

func main(){
	type num float64
	type x struct {
		y,z num
	}

	for {
		type x struct {
			y,z num
		}
	}
}