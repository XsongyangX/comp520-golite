/* The file contains valid unary operator usage. */
package main

type num int
type count num
type quantity count
func main(){
	var magic quantity
	magic = +magic
}