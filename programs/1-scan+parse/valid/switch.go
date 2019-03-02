/* Example of switch statements */
package main

func getMyNumber() int {

	return 100
}

func main () {
	var x int
	
	// numerical cases
	switch x {
	default: print(x)
	case 0, 1, 2, 3: x++
	case 4,5,6,7: x--
	}

	// missing switch expression
	switch {
		case x == 1: print(1)
	}

	// short declaration and missing switch expression
	switch x = getMyNumber(); {
	case x<0: print(x)
	default: print(x)
	}
}
