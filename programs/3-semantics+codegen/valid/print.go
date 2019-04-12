//~25525551966
//~+1.200000e-001+2.000000e+011
//~truefalse
//~76
//~hello 
//~hello \n12341001 2 3 4 100
//~
//~end
package main

func main() {

	// int
	print(255)
	print(0377)
	print(0xCAFE)
	print("\n")

	// float
	print(.12)
	print(200000000000.0)
	print("\n")

	// boolean
	print(true)
	print(false)
	print("\n")

	// runes
	print('L')
	print("\n")

	// strings
	print("hello \n")
	print(`hello \n`)

	// many args
	print(1, 2, 3, 4, 100)
	println(1, 2, 3, 4, 100)

	// no arg
	print()
	println()

	print("end")
}

