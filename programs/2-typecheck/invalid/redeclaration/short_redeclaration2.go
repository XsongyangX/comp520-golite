/* Short redeclaration in the same scope */
package main

func main(){
	if true {
		a := "0"
		a := a
	}
}