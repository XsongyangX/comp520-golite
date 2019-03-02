package main

var seed int = 5;

func main() {
	var randInt int;
	randInt = rand();
	println(randInt);
	randInt = rand();
	println(randInt);
	randInt = rand();
	println(randInt);
}

func rand() int{
	var m, a, c int = 2^(32), 1664525, 1013904223;
	seed++;
	return (a*seed + c) % m;
}