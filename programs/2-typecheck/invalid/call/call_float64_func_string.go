/* The file contains incorrect function call typing. */
package main

func foo(arg string){}
func main(){
	var magic float64
	foo(magic)
}