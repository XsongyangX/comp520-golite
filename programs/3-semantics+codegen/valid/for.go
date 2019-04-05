//~in loop
//~012345678910111213141516171819
//~the second loop
package main

func main() {

	// while loop
	var a, b int
	for a+b == 0 {
		println("in loop")
		a++
	}

	// for loop
	for i := 0; i < 20; i++ {
		print(i)
	}
	println()

	for j, k := 0, 1; j < k; j, k = k, j {
		println("the second loop")

		if j < k {
			continue
		}

		println("after continue")
	}

}

