/* The file contains invalid assignment of declared vars. */
package main

type num int
type Date struct{ day string; month string; year int; }
func main(){
	var magic num
	var m Date
	magic = m
}