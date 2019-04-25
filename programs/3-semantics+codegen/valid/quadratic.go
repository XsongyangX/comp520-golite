//~The roots of x^2+2x-10 are:
//~+2.316625e+000
//~-4.316625e+000

package main

func sqrt(x float64) float64 {

	var ans float64 = 1.0

	for i := 0; i < 32; i++ {
		ans = (ans + x/ans) / 2.0
	}

	return ans
}

func quadraticRoots(a float64, b float64, c float64) [2]float64 {
	var discriminant = b*b - 4.0*a*c

	// negative discriminant
	if discriminant < 0.0 {
		var error [2]float64
		error[0] = 0.0
		error[1] = 0.0
		return error
	}

	discriminant = sqrt(discriminant)

	var roots [2]float64
	roots[0] = (-b + discriminant) / (2.0 * a)
	roots[1] = (-b - discriminant) / (2.0 * a)
	return roots
}

func main() {
	println("The roots of x^2+2x-10 are:")
	var answer = quadraticRoots(1.0,2.0,-10.0)
	println(answer[0])
	println(answer[1])
}

