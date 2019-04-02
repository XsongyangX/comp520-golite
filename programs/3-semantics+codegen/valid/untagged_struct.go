//~welcome
package main

func takeAStruct(a struct {
	head      string
	shoulders string
}) {
	print("welcome")
}

func main() {
	var myBeauty struct {
		head      string
		shoulders string
	}
	myBeauty.head = "clean"
	myBeauty.shoulders = "smooth"
	takeAStruct(myBeauty)
}
