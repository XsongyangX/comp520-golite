/* Invalid print */
package main

var i struct{
	s string
}

func main() {
	print(i)
}
