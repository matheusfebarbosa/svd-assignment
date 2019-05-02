#include "evaluate.hpp"

#include <iostream>
#include <random>

vector<Dataset*> k_fold(Dataset *ds, unsigned int k){
	vector<Dataset*> folds;

	for(unsigned int i=0;i<k;i++){
		folds.push_back(new Dataset());
	}

	default_random_engine generator{42};
  	uniform_int_distribution<int> distribution(0,k-1);

	for(auto p : ds->events()){
		int u = p.first.first;
		int i = p.first.second;
		double rating = p.second;

		string user = ds->get_user(u);
		string item = ds->get_item(i);
		int fold = distribution(generator);
		
		folds[fold]->add_event(make_pair(make_pair(user,item),rating));
	}

	return folds;
}

double k_fold_evaluate(SVD *svd, Dataset *ds, int n_folds, bool verbose){
	if(verbose){
		cerr <<"---------------------------------------------" << endl;
		cerr <<"Number of Folds: " << n_folds << endl;
	}

	vector<Dataset*> folds = k_fold(ds,n_folds);
	double mean_rmse = 0;

	if(verbose){
		cerr << "Folds generated!!!" << endl;

		for(int i = 0; i<n_folds; i++){
			cerr << "size(fold[" << i << "]) -> " << folds[i]->events().size() << endl;
		}

		cerr <<"---------------------------------------------" << endl;
	}

	for(int i = 0; i<n_folds; i++){
		if(verbose)
			cerr << "Running test on fold " << i << endl;

		vector<Dataset*> train_folds;

		copy(folds.begin(),folds.begin()+i,back_inserter(train_folds));
		copy(folds.begin()+i+1,folds.end(),back_inserter(train_folds));

		Dataset *merged = merge_datasets(train_folds);

		svd->fit(merged);

		double rmse = svd->rmse(folds[i]);

		mean_rmse += rmse;

		if(verbose){
			cerr << "Test RMSE: " << rmse << endl;
			cerr <<"---------------------------------------------" << endl;
		}

		delete merged;
	}

	for(int i = 0; i<n_folds; i++){
		delete folds[i];
	}

	mean_rmse /= n_folds;

	return mean_rmse;
}