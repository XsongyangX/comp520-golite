/* Valid variable shadowing */
package main

var (
	s struct{
		n int
		m float64
	}
	time int
)

func main(){
	var time int
	for {
		var s string
	}
}