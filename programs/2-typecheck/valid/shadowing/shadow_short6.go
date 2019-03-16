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
	time := 0
	switch {
		default:
			s := "mn"
	}
}