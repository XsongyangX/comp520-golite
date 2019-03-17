/* The file contains valid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer

func main(){
	var x answer
	var y answer
	var magic answer
	magic = x && y

}