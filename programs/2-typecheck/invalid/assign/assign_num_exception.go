/* The file contains invalid assignment of declared vars. */
package main

type num int
type exception string
func main(){
	var magic num
	var m exception
	magic = m
}