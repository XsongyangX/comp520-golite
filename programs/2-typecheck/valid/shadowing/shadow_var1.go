/* Valid variable shadowing */
package main

var s struct{
	n int
	m float64
}

func main(){
	var s float64 = 1.0
}