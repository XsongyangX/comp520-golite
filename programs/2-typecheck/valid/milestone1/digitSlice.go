//~13211
/*Takes a number as input and creates a slice
with all the digits of that number in reverse order*/
package main

func intToSlice(num int) []rune{
	var numSlice []rune;
	for num>0 {
		numSlice = append(numSlice, rune(num%10));
		num /= 10;
	}
	return numSlice
}

func main() {
	slice := intToSlice(11231)

	for i:=0; i<len(slice);i++ {
		print(slice[i])
	}
}
