/* The file contains incorrect assignments of return values. */
package main
func foo() string {
	return "hello world"
}
func main(){
	var a int; a = foo()
}