#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "submission.hpp"

using namespace std;

int main(int argc, char *argv[]){

	if (argc!=3){
		cout << "Wrong number of arguments!" << endl;
		cout << "How to use: " << endl;
		cout << endl;
		cout << "./tp.out /path/to/ratings.csv /path/to/targets.csv > submission.csv" << endl;
	}

	srand(42);

	Dataset ds(argv[1],0.2);
	SVD svd;

	svd.fit(ds,10,0.001,0.001,100);

	cerr <<"Train RMSE: " << svd.rmse(ds.train()) << endl;
	cerr <<"Train MAE: " << svd.mae(ds.train()) << endl;
	cerr <<"Test RMSE: " << svd.rmse(ds.test()) << endl;
	cerr <<"Test MAE: " << svd.mae(ds.test()) << endl;

	generate_submission(svd, ds, argv[2]);

	return 0;
}