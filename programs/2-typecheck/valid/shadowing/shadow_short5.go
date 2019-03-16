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
	time := 324
	if time < 100{
		s := "op"
	}
}