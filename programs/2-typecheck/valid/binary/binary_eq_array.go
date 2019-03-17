/* The file contains valid binary operator usage. */
package main

type answer bool
type safe answer
type exist answer
type real float64
type positive real
type non_zero positive
type letter rune
type character letter
type symbol character
type num int
type count num
type quantity count
type error string
type log error
type report log
type compound struct { x int; y float64;}
type composed struct { c compound; s string;}
type group struct { c1 compound; c2 composed;}
type inbox [100]string
type mailbox [3]inbox
type postoffice [5]mailbox
func main(){
	var x array
	var y array
	var magic bool
	magic = x == y

}