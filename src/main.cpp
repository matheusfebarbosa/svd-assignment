#include <iostream>

#include "svd.hpp"
#include "dataset.hpp"
#include "submission.hpp"

#define USE_CUSTOM true

const struct param{
	const int k = 100;
	const double lr = 0.005;
	const double reg = 0.1;
	const int epochs = 50;
	const double dist_mean = 0;
	const double dist_deviation = 0.01;
	const double test_ratio = 0.3;
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

		Dataset ds(argv[1], param.test_ratio);

		SVD *svd;

		if(USE_CUSTOM){
			svd = new SVD(param.k,param.lr, param.reg, 
				param.dist_mean, param.dist_deviation, param.epochs);
		}else{
			svd = new SVD();
		}

		svd->fit(ds,true);

		cerr <<"------------------------------------------------------" << endl;
		cerr <<"Train RMSE: " << svd->rmse(ds.train()) << endl;
		cerr <<"Train MSE: " << svd->mse(ds.train()) << endl;
		cerr <<"Train MAE: " << svd->mae(ds.train()) << endl;
		cerr <<"~~~~~~~~~~~~~~~~~~~~" << endl;
		cerr <<"Test RMSE: " << svd->rmse(ds.test()) << endl;
		cerr <<"Test MSE: " << svd->mse(ds.test()) << endl;
		cerr <<"Test MAE: " << svd->mae(ds.test()) << endl;
		cerr <<"~~~~~~~~~~~~~~~~~~~~" << endl;
		cerr <<"Baseline for train" << endl;
		svd->check_baseline(ds.train());
		cerr <<"Baseline for test" << endl;
		svd->check_baseline(ds.test());

		delete svd;

	} else if (argc >= 3) {

		Dataset ds(argv[1],0.0);

		SVD *svd;
		
		if(USE_CUSTOM){
			svd = new SVD(param.k,param.lr, param.reg, 
				param.dist_mean, param.dist_deviation, param.epochs);
		}else{
			svd = new SVD();
		}

		svd->fit(ds,true);

		generate_submission(svd, ds, argv[2]);

		delete svd;
	}
	
	return 0;
}