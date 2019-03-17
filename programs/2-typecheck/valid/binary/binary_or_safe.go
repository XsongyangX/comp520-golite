/* The file contains valid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer
func main(){
	var x safe
	var y safe
	var magic safe
	magic = x || y

}