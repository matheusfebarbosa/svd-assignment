#include <iostream>

#include "svd.hpp"
#include "dataset.hpp"
#include "submission.hpp"
#include "evaluate.hpp"

#define USE_CUSTOM true
#define VERBOSE_TRAIN false
#define VERBOSE_EVAL true
#define N_FOLDS 5

const struct param{
	const int k = 20;
	const double lr_m = 0.005;
	const double lr_b = 0.0045;
	const double reg = 0.05;
	const int epochs = 20;
	const double dist_mean = 0.00;
	const double dist_deviation = 0.01;
	const bool bias = true;
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

		Dataset *ds = new Dataset(argv[1]);

		SVD *svd;

		if(USE_CUSTOM){
			svd = new SVD(param.k, param.reg, param.lr_m, param.dist_mean, 
				param.dist_deviation, param.epochs, param.bias,param.lr_b,VERBOSE_TRAIN);
		}else{
			svd = new SVD();
		}

		cerr <<"------------------------------------------------------" << endl;
		cerr <<"Validating model..." << endl;
		
		double mean_rmse = k_fold_evaluate(svd, ds, N_FOLDS, VERBOSE_EVAL);

		cerr <<"Test Finished!!!" << endl;
		cerr <<"Mean RMSE: " << mean_rmse << endl;
		cerr <<"------------------------------------------------------" << endl;

		delete svd;
		delete ds;

	} else if (argc >= 3) {

		Dataset *ds = new Dataset(argv[1]);

		SVD *svd;
		
		if(USE_CUSTOM){
			svd = new SVD(param.k, param.reg, param.lr_m, param.dist_mean, 
				param.dist_deviation, param.epochs, param.bias,param.lr_b,VERBOSE_TRAIN);
		}else{
			svd = new SVD();
		}

		svd->fit(ds);

		generate_submission(svd, ds, argv[2]);

		delete svd;
		delete ds;
	}
	
	return 0;
}