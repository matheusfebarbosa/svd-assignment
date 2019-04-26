#include <iostream>

#include "svd.hpp"
#include "dataset.hpp"
#include "submission.hpp"

#define USE_CUSTOM true
#define VERBOSE_TRAIN true
#define N_FOLDS 5

const struct param{
	const int k = 30;
	const double lr_m = 0.005;
	const double lr_b = 0.0045;
	const double reg = 0.1;
	const int epochs = 20;
	const double dist_mean = 0;
	const double dist_deviation = 0.01;
	const double test_ratio = 0.3;
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
				param.dist_deviation, param.epochs, param.bias,param.lr_b);
		}else{
			svd = new SVD();
		}


		cerr <<"------------------------------------------------------" << endl;
		cerr <<"Validating model..." << endl;
		cerr <<"Number of Folds: " << N_FOLDS << endl;

		vector<Dataset*> folds = k_fold(ds,N_FOLDS);
		double mean_rmse = 0;

		cerr << "Folds generated!!!" << endl;

		for(int i = 0; i<N_FOLDS; i++){
			cerr << "size(fold[" << i << "]) -> " << folds[i]->events().size() << endl;
		}

		cerr <<"------------------------------------------------------" << endl;

		for(int i = 0; i<N_FOLDS; i++){
			for(int j = 0; j<N_FOLDS; j++){
				if(i!=j){
					svd->fit(folds[i],false);

					double rmse = svd->rmse(folds[j]);

					mean_rmse += rmse;

					cerr << "Run " << i << "->" << j << ";" << endl;
					cerr << "RMSE " << rmse << endl;
 				}
			}
		}

		mean_rmse /= N_FOLDS*(N_FOLDS-1);

		cerr <<"------------------------------------------------------" << endl;
		cerr <<"Test Finished!!!" << endl;
		cerr <<"Mean RMSE: " << mean_rmse << endl;

		delete svd;

	} else if (argc >= 3) {

		Dataset *ds = new Dataset(argv[1]);

		SVD *svd;
		
		if(USE_CUSTOM){
			svd = new SVD(param.k, param.reg, param.lr_m, param.dist_mean, 
				param.dist_deviation, param.epochs, param.bias,param.lr_b);
		}else{
			svd = new SVD();
		}

		svd->fit(ds,VERBOSE_TRAIN);

		generate_submission(svd, ds, argv[2]);

		delete svd;
	}
	
	return 0;
}