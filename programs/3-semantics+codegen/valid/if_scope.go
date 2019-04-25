//~branch 3

package main

func main() {

	if a := 0; false || a == 1000 { // the equality is to prevent an used variable error
		print("branch 1")
	} else if a := true; !a { // shadowing of a
		print("branch 2")
	} else if a := a && a; a { // shadowing of a, using previous value of a
		print("branch 3")
	} else {
		print("else branch")
	}
}
