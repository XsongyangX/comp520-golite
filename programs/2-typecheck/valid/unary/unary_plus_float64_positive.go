/* The file contains valid unary operator usage. */
package main

type real float64
type positive real
type non_zero positive
func main(){
	var magic float64
	var x positive
	magic = +x
}