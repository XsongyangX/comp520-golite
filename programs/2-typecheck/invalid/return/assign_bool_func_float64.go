/* The file contains incorrect assignments of return values. */
package main
func foo() float64 {
	return 10.32
}
func main(){
	var a bool; a = foo()
}