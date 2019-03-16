/* The file contains incorrect function call typing. */
package main

func foo(arg rune){}
func main(){
	var magic string
	foo(magic)
}