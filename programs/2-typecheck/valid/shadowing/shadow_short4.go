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
	time := 98
	for i:=0 ; i < 100; i++ {
		s := ""
	}
}