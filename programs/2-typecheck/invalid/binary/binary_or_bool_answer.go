/* The file contains invalid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer

func main(){
	var x answer
	var y answer
	var magic bool
	magic = x || y

}