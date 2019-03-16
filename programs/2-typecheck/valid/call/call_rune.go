/* The file contains correct function call typing. */
package main

func foo(arg rune){}
func main(){
	var x rune
	foo(x)
}