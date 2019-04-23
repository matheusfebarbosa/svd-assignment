#ifndef SVD_H
#define SVD_H 

#include "dataset.hpp"

#include <vector>
#include <utility>

using namespace std;

class SVD{

private:
	//matrices
	double **U_;
	double **V_;
	unsigned int n_users_;
	unsigned int n_items_;
	unsigned int f_;
	double dist_mean_;
	double dist_deviation_;
	//training parameters
	double lr_;
	double reg_;
	int epochs_;
	//self bias
	bool bias_;
	vector<double> user_bias_;
	vector<double> item_bias_;
	double global_mean_;


	void init_matrices(unsigned int r, unsigned int c, unsigned int f);
	void generate_global_mean(vector<pair<pair<int,int>,double>> events);
	void generate_user_bias(vector<pair<pair<int,int>,double>> events);
	void generate_item_bias(vector<pair<pair<int,int>,double>> events);
	void randomize_matrices();

public:

	SVD(unsigned int n_factors = 100, double lr = 0.005, double reg = 0.1, double dist_mean = 0, 
		double dist_deviation = 0.01, unsigned int epochs = 50){
		U_ = NULL;
		V_ = NULL;
		f_ = n_factors;
		lr_ = lr;
		reg_ = reg;
		epochs_ = epochs;
		dist_mean_ = dist_mean;
		dist_deviation_ = dist_deviation;
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

	void fit(Dataset &ds, bool bias);
	double predict(int user, int item);
	double interaction(int user, int item);
	double fixed(int user, int item);
	double* user_f(int user);
	double* item_f(int item);
	double mse(vector<pair<pair<int,int>,double>> events);
	double mae(vector<pair<pair<int,int>,double>> events);
	double rmse(vector<pair<pair<int,int>,double>> events);
	bool is_biased();
	void check_baseline(vector<pair<pair<int,int>,double>> events);

};

#endif