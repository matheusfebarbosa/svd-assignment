#include <iostream>

#include "svd.hpp"
#include "dataset.hpp"
#include "submission.hpp"

const struct param{
	const int k = 10;
	const double lr = 0.001;
	const double reg = 0.05;
	const int epochs = 20;
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

	srand(4);

	if(argc == 2){

		Dataset ds(argv[1],0.20);
		SVD svd(param.k,param.lr, param.reg, param.epochs);

		svd.fit(ds,false);

		cerr <<"------------------------------------------------------" << endl;
		cerr <<"Train RMSE: " << svd.rmse(ds.train()) << endl;
		cerr <<"Train MSE: " << svd.mse(ds.train()) << endl;
		cerr <<"Train MAE: " << svd.mae(ds.train()) << endl;
		cerr <<"~~~~~~~~~~~~~~~~~~~~" << endl;
		cerr <<"Test RMSE: " << svd.rmse(ds.test()) << endl;
		cerr <<"Test MSE: " << svd.mse(ds.test()) << endl;
		cerr <<"Test MAE: " << svd.mae(ds.test()) << endl;
		cerr <<"~~~~~~~~~~~~~~~~~~~~" << endl;
		cerr <<"Baseline for train" << endl;
		svd.check_baseline(ds.train());
		cerr <<"Baseline for test" << endl;
		svd.check_baseline(ds.test());

	}else if (argc >= 3) {

		Dataset ds(argv[1],0.0);
		SVD svd(param.k,param.lr, param.reg, param.epochs);

		svd.fit(ds,true);

		generate_submission(svd, ds, argv[2]);
	}
	
	return 0;
}