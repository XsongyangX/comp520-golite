/* Shadowing a simple statement */
package main

func main(){
	if a := 3; true{
		var a = "op"
	}
}