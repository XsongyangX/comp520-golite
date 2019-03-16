/* The file contains incorrect function call typing. */
package main

func foo(arg float64){}
func main(){
	var magic bool
	foo(magic)
}