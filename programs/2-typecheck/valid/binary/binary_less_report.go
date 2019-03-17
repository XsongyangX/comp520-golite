/* The file contains valid binary operator usage. */
package main

type error string
type log error
type report log

func main(){
	var x report
	var y report
	var magic bool
	magic = x < y

}