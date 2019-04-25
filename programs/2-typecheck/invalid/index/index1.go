/* Invalid indexing */
package main

func main(){
	var e struct { n []int; }
	e[0] = 1
}