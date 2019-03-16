/* The file contains invalid assignment of declared vars. */
package main

type exception string
type num int
func main(){
	var magic exception
	var m num
	magic = m
}