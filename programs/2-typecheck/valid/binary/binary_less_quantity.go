/* The file contains valid binary operator usage. */
package main

type num int
type count num
type quantity count

func main(){
	var x quantity
	var y quantity
	var magic bool
	magic = x < y

}