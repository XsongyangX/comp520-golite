/* Invalid top level declaration: if statement in global scope */
package main

if true == true {
	print("Hello world")
}

var s string