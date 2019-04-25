/* The file contains invalid assignment of declared vars. */
package main

type Date struct{ day string; month string; year int; }
type long float64
func main(){
	var magic Date
	var m long
	magic = m
}