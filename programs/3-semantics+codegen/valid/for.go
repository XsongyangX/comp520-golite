//~in loop
//~012345678910111213141516171819
//~the second loop
//~hi
//~hihi
//~hi
//~hihi
//~hi
//~0102030405060
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

	for x, y, z := 0, 1, 2; x+y+z < 10; x, y, z = y+1, z+1, x+1 {
		println("hi")
		if x+y+z == 9 {
			continue
		}
		println("hihi")
	}

	var i = 0
	for ; i < 100; i+=10 {
		print(i)
		if i > 50 {
			break
		}
	}

}

