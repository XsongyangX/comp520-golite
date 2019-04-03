//~true
//~false
package main

func main(){
	var a,b struct {
		f int
		_ float64
	}
	println(a == b)
	
	b.f = 1
	println(a == b)
}
