/* The file contains incorrect function call typing. */
package main

func foo(arg int){}
func main(){
	var magic rune
	foo(magic)
}