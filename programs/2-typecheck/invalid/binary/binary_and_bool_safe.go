/* The file contains invalid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer

func main(){
	var x safe
	var y safe
	var magic bool
	magic = x && y

}