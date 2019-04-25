//~0
//~1
//~0
package main

func main() {
	var a, b [5]int
	b = a    // copies everything
	a[0] = 1 // local changes to a

	var c, d []int
	c = append(c, 0)
	d = c    // copy header
	c[0] = 1 // changes d too, because the underlying array is the same

	var e, f struct{ f int; }
	f = e   // copy all fields
	e.f = 1 // local changes

	println(b[0])
	println(d[0])
	println(f.f)
}

