/* The file contains valid unary operator usage. */
package main

type letter rune
type character letter
type symbol character
func main(){
	var magic letter
	var x rune
	magic = -x
}