package main

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

func main() {
	var test []int;
	test = append(test, 5)
	test = append(test, 3)
	test = append(test, 7)
	test = append(test, 9)
	test = append(test, 0)
	result := mergeSort(test)
	for i:= 0; i < len(result); i++ {
		print(result[i])
		print(" ")
	}
}
