/* The file contains incorrect assignments of return values. */
package main
func foo() int {
	return 300
}
func main(){
	var a float64; a = foo()
}