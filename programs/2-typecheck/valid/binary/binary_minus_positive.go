/* The file contains valid binary operator usage. */
package main

type real float64
type positive real
type non_zero positive

func main(){
	var x positive
	var y positive
	var magic positive
	magic = x -y

}