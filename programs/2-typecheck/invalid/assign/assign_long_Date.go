/* The file contains invalid assignment of declared vars. */
package main

type long float64
type Date struct{ day string; month string; year int; }
func main(){
	var magic long
	var m Date
	magic = m
}