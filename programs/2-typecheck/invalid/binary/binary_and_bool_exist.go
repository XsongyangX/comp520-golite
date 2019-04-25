/* The file contains invalid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer

func main(){
	var x exist
	var y exist
	var magic bool
	magic = x && y

}