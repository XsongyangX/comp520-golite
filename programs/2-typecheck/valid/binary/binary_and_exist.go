/* The file contains valid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer

func main(){
	var x exist
	var y exist
	var magic exist
	magic = x && y

}