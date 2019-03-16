/* The file contains valid assignment declared vars. */
package main

type Date struct{ day string; month string; year int; }
func main(){
	var magic Date
	var m Date
	magic = m
}