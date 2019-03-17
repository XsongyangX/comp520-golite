/* The file contains valid unary operator usage. */
package main

type answer bool
type safe answer
type exist answer
func main(){
	var magic exist
	magic = !magic
}