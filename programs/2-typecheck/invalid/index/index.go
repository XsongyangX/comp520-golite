/* Invalid indexing */
package main

func main(){
	var e struct { n []int; }
	print(e[0])
}