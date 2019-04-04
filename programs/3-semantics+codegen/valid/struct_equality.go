//~true
//~false
//~true
//~false
//~true
//~false
package main

func main(){
	var a,b struct {
		f int
		_ float64
	}
	println(a == b)
	
	b.f = 1
	println(a == b)

	// more complicated cases
	var x,y struct {
		head struct {
			i int
			j int
		}
		tail struct {
			i int
			j int
		}
	}
	println(x == y)

	x.head.i = 10
	println(x == y)

	var m,n [10] struct{
		a [10]int
		b [10]int
	}
	println(m == n)

	m[5].a[2] = 19
	println(m == n)
}
