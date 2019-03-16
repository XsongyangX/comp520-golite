/* The file contains incorrect assignments of return values. */
package main
func foo() bool {
	return false
}
func main(){
	var a int; a = foo()
}