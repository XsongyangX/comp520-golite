/* The file contains valid binary operator usage. */
package main

type letter rune
type character letter
type symbol character

func main(){
	var x symbol
	var y symbol
	var magic symbol
	magic = x %y

}