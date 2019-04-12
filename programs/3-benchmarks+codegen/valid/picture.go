//~3, 3, 3
//~1, 1, 1
//A library of functions for making images black and white or blurring them
package main

type Pixel struct{
 	r, g, b int;
}

type Image struct {
	xLen, yLen int;
	grid[][] Pixel;
}

func makePixel(r, g, b int) Pixel {
	var pix Pixel;
	pix.r = r;
	pix.g = g;
	pix.b = b;
	return pix;
}

func makeImage(xLen, yLen int) Image {
	var picture Image;
	picture.xLen = xLen;
	picture.yLen = yLen;
	var grid[][] Pixel;
	picture.grid = grid;
	for i:= 0; i< xLen; i++ {
		var row[] Pixel;
		for i:= 0; i< yLen; i++ {
			row = append(row, makePixel(0, 0, 0)); 
		}
		picture.grid = append(picture.grid, row) 
	}
	return picture;
}

func blackAndWhite(picture Image) Image {
	var temp int;
	var newPic Image;
	newPic = makeImage(picture.xLen, picture.yLen);
	for i:=0; i < picture.xLen; i++ {
		for j:=0; j < picture.yLen; j++ {
			temp = 0;
			temp += picture.grid[i][j].r;
			temp += picture.grid[i][j].g;
			temp += picture.grid[i][j].b;
			newPic.grid[i][j].r = temp / 3;
			newPic.grid[i][j].g = temp / 3;
			newPic.grid[i][j].b = temp / 3;
		} 
	}
	return newPic;
}

func blur(picture Image, radius int) Image {
	var tmpR, tmpG, tmpB, tmpCount int;
	var newPic Image;
	newPic = makeImage(picture.xLen, picture.yLen);
	for i:=0; i < picture.xLen; i++ {
		for j:=0; j < picture.yLen; j++ {
			tmpCount = 0;
			tmpR = 0;
			tmpG = 0;
			tmpB = 0;
			for tmpi:=i - radius; tmpi < i + radius; tmpi++ {
				for tmpj:=j - radius; tmpi < j + radius; tmpj++ {
					if(!(tmpi > picture.xLen || tmpi < 0 || tmpj > picture.yLen || tmpj < 0)){
						tmpR += picture.grid[tmpi][tmpj].r
						tmpG += picture.grid[tmpi][tmpj].g
						tmpB += picture.grid[tmpi][tmpj].b
						tmpCount++;
					}
				}
			}
			newPic.grid[i][j] = makePixel(tmpR/tmpCount, tmpG/tmpCount, tmpB/tmpCount);
		} 
	}
	return newPic;
}


func main() {
	var pic Image;
	pic = makeImage(3, 3);
	pic.grid[0][0] = makePixel(3, 4, 2);
	pic.grid[1][0] = makePixel(1, 1, 1);
	pic.grid[2][0] = makePixel(10, 1, 1);
	pic.grid[0][1] = makePixel(1, 1, 1);
	pic.grid[1][1] = makePixel(1, 1, 1);
	pic.grid[2][1] = makePixel(1, 1, 1);
	pic.grid[0][2] = makePixel(1, 1, 1);
	pic.grid[1][2] = makePixel(1, 1, 1);
	pic.grid[2][2] = makePixel(10, 1, 1);
	pic = blackAndWhite(pic);
	print(pic.grid[0][0].r, ", ", pic.grid[0][0].g, ", ", pic.grid[0][0].b, "\n");
	print(pic.grid[0][1].r, ", ", pic.grid[0][1].g, ", ", pic.grid[0][1].b, "\n");
}