/* The file contains invalid assignment of declared vars. */
package main

type num int
type long float64
func main(){
	var magic num
	var m long
	magic = m
}