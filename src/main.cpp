#include <iostream>

#include "svd.hpp"
#include "dataset.hpp"
#include "submission.hpp"

const struct param{
	const int k = 10;
	const double lr = 0.001;
	const double reg = 0.2;
	const int epochs = 50;
} param;

using namespace std;

int main(int argc, char *argv[]){

	if (argc<2){
		cout << "Wrong number of arguments!" << endl;
		cout << "How to use: " << endl;
		cout << endl;
		cout << "Generate submission: " << endl;
		cout << "./recommender /path/to/ratings.csv /path/to/targets.csv > submission.csv" << endl;
		cout << endl;
		cout << "Just test model: " << endl;
		cout << "./recommender /path/to/ratings.csv" << endl;
		return 1;
	}

	srand(20);

	if(argc == 2){

		Dataset ds(argv[1],0.25);
		SVD svd(param.k,param.lr, param.reg, param.epochs);

		svd.fit(ds);

		cerr <<"Train RMSE: " << svd.rmse(ds.train()) << endl;
		cerr <<"Train MSE: " << svd.mse(ds.train()) << endl;
		cerr <<"Train MAE: " << svd.mae(ds.train()) << endl;
		cerr <<"Test RMSE: " << svd.rmse(ds.test()) << endl;
		cerr <<"Test MSE: " << svd.mse(ds.test()) << endl;
		cerr <<"Test MAE: " << svd.mae(ds.test()) << endl;

	}else if (argc >= 3) {

		Dataset ds(argv[1],0.0);
		SVD svd(param.k,param.lr, param.reg, param.epochs);

		svd.fit(ds);

		generate_submission(svd, ds, argv[2]);	

	}
	
	return 0;
}