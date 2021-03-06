/* This program will print a spinning bar. It will terminate after
100 iterations. */

package main

func spinningBar() {

	var i = 0
	var j = 0
	var state [4]rune
	state[0] = '/'
	state[1] = '-'
	state[2] = '\\'
	state[3] = '|'

	for true {
		println(string(state[i]))
		i++
		i %= 4
		j++

		if j == 100 {
			break
		}
	}
}

func main() {
	spinningBar()
}
