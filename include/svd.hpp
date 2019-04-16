#ifndef SVD_H
#define SVD_H 

#include "dataset.hpp"

#include <vector>
#include <utility>

using namespace std;

class SVD{

private:
	double **U_;
	double **V_;
	double lr_;
	double reg_;
	int epochs_;
	int f_;
	int r_;
	int c_;

	void init_matrices(int r, int c, int f);
	void randomize();

public:
	SVD(int n_factors, double lr, double reg, int epochs){
		U_ = NULL;
		V_ = NULL;
		f_ = n_factors;
		lr_ = lr;
		reg_ = reg;
		epochs_ = epochs;
	}

	~SVD(){
		if (U_!=NULL){
			for (int i = 0; i < r_; i++)
			    delete [] U_[i];
			delete [] U_;
		}
		if (V_!=NULL){
			for (int i = 0; i < c_; i++)
			    delete [] V_[i];
			delete [] V_;
		}
	}

	void fit(Dataset &ds);
	double predict(int user, int factor);
	double* predict(int user);
	double* user_f(int user);
	double* item_f(int item);
	double mse(vector<pair<pair<int,int>,double>> events);
	double mae(vector<pair<pair<int,int>,double>> events);
	double rmse(vector<pair<pair<int,int>,double>> events);

};

#endif