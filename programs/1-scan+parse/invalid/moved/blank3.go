/* Using _ as a struct */
package main

var _ struct {
	u int
	v int
}

func main(){
	_.u = 3
}
