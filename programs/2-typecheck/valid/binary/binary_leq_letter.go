/* The file contains valid binary operator usage. */
package main

type letter rune
type character letter
type symbol character

func main(){
	var x letter
	var y letter
	var magic bool
	magic = x <= y

}