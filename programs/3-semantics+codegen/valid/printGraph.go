//~|~ ~ O O O
//~|~ ~ O O O
//~|~ ~ ~ O O
//~|~ ~ ~ O ~
//~|~ ~ ~ O ~
//~|_____

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
			curx += (xmax-xmin)/5.0;

		}
		curx = xmin;
		cury -= (ymax-ymin)/5.0;
		println("");
	}
}

func main() {
	printGraph(-1.0, 1.0, -1.0, 1.0, 1.0);
}
