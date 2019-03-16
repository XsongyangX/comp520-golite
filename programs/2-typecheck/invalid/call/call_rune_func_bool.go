/* The file contains incorrect function call typing. */
package main

func foo(arg bool){}
func main(){
	var magic rune
	foo(magic)
}