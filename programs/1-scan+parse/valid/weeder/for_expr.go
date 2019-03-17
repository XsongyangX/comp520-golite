/* Invalid for loop */
package main

func foo(){}

func main(){
	for i:=0; true; foo(){

	}
}
