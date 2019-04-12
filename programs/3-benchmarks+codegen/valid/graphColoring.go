//~false
package main

type node struct{
	neighbors[] node;
	color int;
	visited bool;
}

type graph struct{
	nodes[] node;
	directed bool;
}


func twoColor(myNode node) bool{
	myNode.visited = true;
	var badNodes bool = false;
	for i:= 0; i < len(myNode.neighbors); i++ {
		if(myNode.neighbors[i].color == myNode.color){
			return false;
		} else if (!(myNode.neighbors[i].visited)) {
			if(myNode.color == 1){
				myNode.neighbors[i].color = 2;
			} else {
				myNode.neighbors[i].color = 1;
			}
			badNodes = (badNodes || twoColor(myNode.neighbors[i]));
		}
	}
	return badNodes;
}

func twoColorable(myGraph graph) bool{
	if(len(myGraph.nodes) == 0){
		print("Empty graph!\n");
		return true;
	}
	var startNode node;
	startNode = myGraph.nodes[0];
	startNode.color = 1;
	return twoColor(startNode);
	
	
}

func main() {
	var a, b, c node;
	var triangleGraph graph;
	a.neighbors = append(a.neighbors, b);
	a.neighbors = append(a.neighbors, c);
	b.neighbors = append(b.neighbors, a);
	b.neighbors = append(b.neighbors, c);
	c.neighbors = append(c.neighbors, a);
	c.neighbors = append(c.neighbors, b);
	triangleGraph.nodes = append(triangleGraph.nodes, a)
	triangleGraph.nodes = append(triangleGraph.nodes, b)
	triangleGraph.nodes = append(triangleGraph.nodes, c)
	print(twoColorable(triangleGraph));
	
}
