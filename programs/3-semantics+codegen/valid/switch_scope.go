//~no case

package main

func main() {

	switch x := "no"; {
	default:
		var x = 10
		print(x)
	case x == "yes":
		var x = 'k'
		print(x)
	case x == "no":
		var x float64 = 3.0
		_ = x
		print("no case")
	}

}
