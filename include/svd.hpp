#ifndef SVD_H
#define SVD_H 

#include "dataset.hpp"

#include <vector>
#include <utility>

using namespace std;

class SVD{

private:
	Dataset *ds_;
	//matrices
	double **U_;
	double **V_;
	unsigned int n_users_;
	unsigned int n_items_;
	unsigned int f_;
	double dist_mean_;
	double dist_deviation_;
	//training parameters
	double lr_m_;
	double lr_b_;
	double reg_;
	int epochs_;
	//self bias
	bool bias_;
	vector<double> user_bias_;
	vector<double> item_bias_;
	double global_mean_;
	//verbose train
	bool verbose_;


	void init_matrices(unsigned int r, unsigned int c, unsigned int f);
	void generate_global_mean(vector<pair<pair<int,int>,double>> events);
	void randomize_matrices();

public:

	SVD(unsigned int n_factors = 100,double reg = 0.1, double lr_m = 0.005, double dist_mean = 0, 
		double dist_deviation = 0.01, unsigned int epochs = 50, bool bias=true, double lr_b = 0.005,
		bool verbose = true){
		U_ = NULL;
		V_ = NULL;
		f_ = n_factors;
		lr_m_ = lr_m;
		lr_b_ = lr_b;
		reg_ = reg;
		epochs_ = epochs;
		dist_mean_ = dist_mean;
		dist_deviation_ = dist_deviation;
		bias_ = bias;
		verbose_ = verbose;
	}

	~SVD(){
		if (U_!=NULL){
			for (unsigned int i = 0; i < n_users_; i++)
			    delete [] U_[i];
			delete [] U_;
		}
		if (V_!=NULL){
			for (unsigned int i = 0; i < n_items_; i++)
			    delete [] V_[i];
			delete [] V_;
		}
		user_bias_.clear();
		item_bias_.clear();
	}

	void fit(Dataset *ds);
	double predict(int user, int item);
	double interaction(int user, int item);
	double fixed(int user, int item);
	double mse(Dataset *ds);
	double mae(Dataset *ds);
	double rmse(Dataset *ds);
	
};

#endif