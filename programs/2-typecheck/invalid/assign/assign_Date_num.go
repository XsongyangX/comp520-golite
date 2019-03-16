/* The file contains invalid assignment of declared vars. */
package main

type Date struct{ day string; month string; year int; }
type num int
func main(){
	var magic Date
	var m num
	magic = m
}