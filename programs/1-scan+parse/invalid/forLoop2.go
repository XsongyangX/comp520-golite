/* Invalid for-loop: no initializer, 3 'for' instead */
package main

func main() {
	for for; for; for {
		print("Hello world")
	}
}