/* The file contains invalid assignment of declared vars. */
package main

type long float64
type num int
func main(){
	var magic long
	var m num
	magic = m
}