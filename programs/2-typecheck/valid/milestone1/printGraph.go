/*A small graphing function. Non-distorted results not guaranteed.*/
package main


func functionToGraph(x float64)float64{
	return 3.0*x*x*x - 4.0*x + 2.0;
}


func printGraph(xmin, xmax, ymin, ymax, precision float64){
	var curx, cury float64 = xmin, ymax
	for cury >= ymin{
		
		for curx <= xmax{
			if curx == xmin {
				print("|");
			}else if cury == ymin {
				print("_");
			}else{
				if ((functionToGraph(curx) > cury - precision) && (functionToGraph(curx) < cury + precision)){
					print("O ");
				}else{
					print("~ ");
				}
			}
			curx += (xmax-xmin)/50.0;
			
		}
		curx = xmin;
		cury -= (ymax-ymin)/50.0;
		println("");
	}
}

func main() {
	printGraph(-5.0, 5.0, -5.0, 5.0, 0.7);
}