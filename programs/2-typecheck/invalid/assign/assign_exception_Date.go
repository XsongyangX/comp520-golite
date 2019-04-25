/* The file contains invalid assignment of declared vars. */
package main

type exception string
type Date struct{ day string; month string; year int; }
func main(){
	var magic exception
	var m Date
	magic = m
}