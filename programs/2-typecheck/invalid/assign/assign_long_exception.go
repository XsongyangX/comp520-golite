/* The file contains invalid assignment of declared vars. */
package main

type long float64
type exception string
func main(){
	var magic long
	var m exception
	magic = m
}