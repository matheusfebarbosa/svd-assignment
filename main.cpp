#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "svd.hpp"

using namespace std;

int main(){

	Dataset ds("./dataset/ratings.csv",0.1);
	SVD svd;

	svd.fit(ds,10,0.001,0.005,1000);

	cout <<"Train: " << svd.mse(ds.train()) << endl;
	//cout <<"Test: " << svd.mse(ds.test()) << endl;

	return 0;
}