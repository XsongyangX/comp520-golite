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
//A library of functions for the k-means algorithm on 1-dimensional vectors
package main

type cluster struct{
 	center float64;
	data []float64;
};

func absval(a float64) float64{
	if(a < 0.0){
		return -1.0 * a;
	} else {
		return a
	}
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
  

func main() {
	var clustered []cluster;
	var data[] float64;
	data = append(data, 3.309);
	data = append(data, 9.170);
	data = append(data, 3.922);
	data = append(data, 10.23);
	data = append(data, 5.568);
	data = append(data, 8.914);
	data = append(data, 4.491);
	data = append(data, 9.623);
	data = append(data, 4.670);
	data = append(data, 10.91);
	data = append(data, 4.137);
	data = append(data, 9.796);
	data = append(data, 5.069);
	data = append(data, 8.765);
	data = append(data, 5.361);
	data = append(data, 10.95);
	data = append(data, 4.383);
	data = append(data, 9.951);
	data = append(data, 4.994);
	data = append(data, 9.674);
	data = append(data, 4.380);
	data = append(data, 8.594);
	data = append(data, 4.751);
	data = append(data, 9.787);
	data = append(data, 4.907);
	data = append(data, 11.05);
	data = append(data, 5.180);
	data = append(data, 10.49);
	data = append(data, 4.797);
	data = append(data, 11.22);
	data = append(data, 5.453);
	data = append(data, 10.44);
	data = append(data, 4.428);
	data = append(data, 7.267);
	data = append(data, 4.850);
	data = append(data, 9.611);
	data = append(data, 5.323);
	data = append(data, 9.828);
	data = append(data, 5.473);
	data = append(data, 9.153);
	data = append(data, 2.734);
	data = append(data, 9.352);
	data = append(data, 5.483);
	data = append(data, 10.15);
	data = append(data, 5.294);
	data = append(data, 9.735);
	data = append(data, 4.928);
	data = append(data, 10.42);
	data = append(data, 4.205);
	data = append(data, 9.519);
	data = append(data, 6.449);
	data = append(data, 9.277);
	data = append(data, 3.323);
	data = append(data, 10.08);
	data = append(data, 3.125);
	data = append(data, 9.236);
	data = append(data, 6.271);
	data = append(data, 9.271);
	data = append(data, 5.095);
	data = append(data, 11.78);
	data = append(data, 5.941);
	data = append(data, 10.43);
	data = append(data, 4.078);
	data = append(data, 9.542);
	data = append(data, 5.178);
	data = append(data, 11.55);
	data = append(data, 3.952);
	data = append(data, 11.24);
	data = append(data, 6.322);
	data = append(data, 8.211);
	data = append(data, 5.709);
	data = append(data, 10.92);
	data = append(data, 4.999);
	data = append(data, 9.303);
	data = append(data, 4.257);
	data = append(data, 8.259);
	data = append(data, 3.927);
	data = append(data, 9.207);
	data = append(data, 4.281);
	data = append(data, 9.829);
	data = append(data, 3.794);
	data = append(data, 9.280);
	data = append(data, 6.117);
	data = append(data, 9.839);
	data = append(data, 3.637);
	data = append(data, 11.08);
	data = append(data, 5.378);
	data = append(data, 9.470);
	data = append(data, 6.496);
	data = append(data, 10.63);
	data = append(data, 4.564);
	data = append(data, 9.607);
	data = append(data, 3.790);
	data = append(data, 8.353);
	data = append(data, 5.050);
	data = append(data, 10.58);
	data = append(data, 5.159);
	data = append(data, 10.32);
	data = append(data, 3.766);
	data = append(data, 9.786);
	clustered = cluster1D(data, 2, 5);
	print("Result of clustering:\n");
	for i:=0; i < len(clustered); i++ { 
		print("\tCluster ", i,  ":\n");
		for j:=0; j < len(clustered[i].data); j++ {
			print("\t\t", clustered[i].data[j], "\n");
		}
	}
}
