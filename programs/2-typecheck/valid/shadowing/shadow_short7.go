/* Shadowing a simple statement with short declaration */
package main

func main(){
	switch a := "kk"; {
		default:
			a := 1
	}
}