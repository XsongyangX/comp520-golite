/* Valid shadowing of a var with a type declaration */
package main

var x string

func main(){
	type x int
}
