//~The first 10 elements: 0 1 2 3 4 5 6 7 8 9 
//&0.018s
/* Performs merge sort on a slice of 2.5 million elements (5s run time on standard compiler's generated executable 
The starting slice is already ordered, but in the opposite order. */
// Size of input reduced for debugging
package main

// input size
var element int = 10000

func merge(left []int, right []int) []int {

	var leftIndex, rightIndex = 0, 0
	var length = len(left) + len(right)
	index := leftIndex + rightIndex
	exhaustLeft := false

	var combined []int

	for index < length {

		if left[leftIndex] <= right[rightIndex] {
			combined = append(combined, left[leftIndex])
			leftIndex++
		} else {
			combined = append(combined, right[rightIndex])
			rightIndex++
		}
		if leftIndex == len(left) {
			exhaustLeft = true
			break
		} else if rightIndex == len(right) {
			exhaustLeft = false
			break
		}
		index = leftIndex + rightIndex
	}

	// broke out of the loop because the left slice has been exhausted
	if exhaustLeft {
		for ; rightIndex < len(right); rightIndex++ {
			combined = append(combined, right[rightIndex])
		}
	} else {
		for ; leftIndex < len(left); leftIndex++ {
			combined = append(combined, left[leftIndex])
		}
	}

	return combined
}

func mergeSort(slice []int) []int {
	if len(slice) == 1 {
		return slice
	} else if len(slice) == 2 {
		var newSlice []int
		if slice[0] < slice[1] {
			newSlice = append(newSlice, slice[0])
			newSlice = append(newSlice, slice[1])
		} else {
			newSlice = append(newSlice, slice[1])
			newSlice = append(newSlice, slice[0])
		}
		return newSlice
	}

	// cutting the slice
	var leftHalf []int
	for i := 0; i < len(slice)/2; i++{
		leftHalf = append(leftHalf, slice[i])
	}
	var rightHalf []int
	for i := len(slice)/2; i < len(slice); i++{
		rightHalf = append(rightHalf, slice[i])
	}
	return merge(mergeSort(leftHalf), mergeSort(rightHalf))
}

var test []int

func init() {
	// create a very large slice of number in decreasing order

	for element >= 0 {
		test = append(test, element)
		element--
	}
}

func main() {
	// it should create a slice of the numbers in increasing order
	result := mergeSort(test)
	// only print the first 10 elements for semantics
	print("The first 10 elements: ")
	for i:= 0; i < 10; i++ {
		print(result[i])
		print(" ")
	}
}

