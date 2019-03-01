/* Invalid for loop: var used as initializer of the counter*/
package main

func main() {

	for (var i = 0; i < 10; ++) {
		print(i)
	}
}