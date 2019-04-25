/* The file contains invalid assignment of declared vars. */
package main

type Date struct{ day string; month string; year int; }
type exception string
func main(){
	var magic Date
	var m exception
	magic = m
}