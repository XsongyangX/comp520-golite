/* The file contains incorrect assignments of return values. */
package main
func foo() rune {
	return '3'
}
func main(){
	var a string; a = foo()
}