/* The file contains invalid assignment of declared vars. */
package main

type exception string
type long float64
func main(){
	var magic exception
	var m long
	magic = m
}