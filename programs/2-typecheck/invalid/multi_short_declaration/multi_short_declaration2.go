/* Invalid multiple short declarations: conflicting types */
package main

func main(){
	
	var a,b int
	a, b, c := 1, "2", "hi"
	
}