#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "svd.hpp"

using namespace std;

int main(){

	srand(42);

	Dataset ds("./dataset/ratings.csv",0.2);
	SVD svd;

	svd.fit(ds,10,0.001,0.001,100);

	cout <<"Train RMSE: " << svd.rmse(ds.train()) << endl;
	cout <<"Train MAE: " << svd.mae(ds.train()) << endl;
	cout <<"Test RMSE: " << svd.rmse(ds.test()) << endl;
	cout <<"Test MAE: " << svd.mae(ds.test()) << endl;

	return 0;
}