/* Invalid multiple short declarations: redeclared */
package main

func main(){
	
	var a,b,c int
	a, b, c := b, a, 1
	
	
}