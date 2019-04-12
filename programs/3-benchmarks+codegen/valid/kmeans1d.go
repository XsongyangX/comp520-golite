//~Result of clustering:
//~	Cluster 0:
//~		+1.230000e+000
//~		+5.500000e-001
//~		+2.430000e+000
//~	Cluster 1:
//~		+3.330000e+000
//~		+7.670000e+000
//~		+8.930000e+000
//~	Cluster 2:
//~		+1.000000e+001
//~		+1.150000e+001
//~		+1.370000e+001
//~		+1.210000e+001
//~		+1.130000e+001
package main

type cluster struct{
 	center float64;
	data []float64;
};

func absval(a float64) float64{
	if(a < 0){
		return -1.0 * a;
	} else {
		return a
	}
}

func cluster1D(data[] float64, nClusters, iter int) []cluster {
	var arrCounter int = 0;
	var clusters[] cluster;
	if(nClusters >= len(data)) {
		print("Too many clusters!");
	} else {
		var centers[] float64;
		for i := 0; i < nClusters; i++ {
			centers = append(centers, data[i]);
		}
		for i := 0; i < iter; i++ {
			clusters = group(centers, data);
			clusters = adjust(clusters);
			if (i != iter - 1){
				arrCounter = 0;
				for j := 0; j < len(clusters); j++ {
					centers[j] = clusters[j].center;
					for  k := 0; k < len(clusters[j].data); k++ {
						data[arrCounter] = clusters[j].data[k];
						arrCounter++;
					}
				}
			}
		}
	}
	return clusters;	
}

func group(centers[] float64, data[] float64) []cluster {
	var clusters[] cluster;
	var dummyIndex int;
	var dummyCluster cluster;
	for i := 0; i < len(centers); i++ {
		dummyCluster.center = centers[i];
		clusters = append(clusters, dummyCluster);
	}
	for i := 0; i < len(data); i++ {
		dummyIndex = 0;
		for j := 0; j < len(clusters); j++ { 
			if (absval(clusters[j].center - data[i]) < absval(clusters[dummyIndex].center - data[i])) {
				dummyIndex = j;
			}
		}
		clusters[dummyIndex].data = append(clusters[dummyIndex].data, data[i]);
	}
	return clusters;
}  

func adjust(clusters[] cluster) []cluster {
	var total float64;
	for i := 0; i < len(clusters); i++ {
		for j := 0; j < len(clusters[i].data); j++ {
			total += clusters[i].data[j];
		}
		clusters[i].center = total / float64(len(clusters[i].data));
	}
	return clusters;
}


func main() {
	var clustered []cluster;
	var data[] float64;
	data = append(data, 1.23);
	data = append(data, 2.43);
	data = append(data, 7.67);
	data = append(data, 8.93);
	data = append(data, 10.0);
	data = append(data, 11.5);
	data = append(data, 3.33);
	data = append(data, 13.7);
	data = append(data, 12.1);
	data = append(data, 0.55);
	data = append(data, 11.3);
	clustered = cluster1D(data, 3, 3);
	print("Result of clustering:\n");
	for i:=0; i < len(clustered); i++ { 
		print("\tCluster ", i,  ":\n");
		for j:=0; j < len(clustered[i].data); j++ {
			print("\t\t", clustered[i].data[j], "\n");
		}
	}
}
