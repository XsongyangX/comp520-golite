//~Mountain
//~*
//~*&*
//~*&&*
//~*&&&*

/* Prints ASCII art of mountains */
package main

type Mountain struct {
	top    string
	base   string
	height int
}

func makeMountain(top string, base string, height int) Mountain {
	var m Mountain
	m.top = top
	m.base = base
	m.height = height
	return m
}

func paintMountain(m Mountain) string {
	var art string = ""

	for i := m.height; i > 0; i-- {

		if i == m.height {
			art = art + m.top + "\n"
			continue
		}

		// left side
		art = art + m.top

		for j := 0; j < m.height-i; j++ {
			art = art + m.base
		}

		// right side
		art = art + m.top

		art = art + "\n"
	}
	return art
}

func main() {
	println("Mountain")
	var m Mountain = makeMountain("*", "&", 4)
	print(paintMountain(m))

}
