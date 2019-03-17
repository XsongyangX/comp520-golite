/* Invalid type cast */
package main

type house struct { door string; }

func main(){
	var h house
	var l house = house(h)
}