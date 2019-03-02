/* Produces an error because of package declarations located inside local scopes*/
package main

func main(){
	package test
	print(1)
}