#include <cmath>

#include "dataset.hpp"

using namespace std;

class SVD{

private:
	double **U_;
	double **V_;
	double lr_;
	int f_;
	int r_;
	int c_;

	void init_ur(int r, int c, int f);
	void randomize();

public:
	SVD(){

	}

	~SVD(){
		for (int i = 0; i < r_; i++)
		    delete [] U_[i];
		delete [] U_;
		for (int i = 0; i < c_; i++)
		    delete [] V_[i];
		delete [] V_;
	}

	void fit(Dataset ds, int n_factors, double lr, double reg, int epochs);
	void fit(double **matrix, int r, int c, int n_factors, double lr, double reg, int epochs);
	double predict(int user, int factor);
	double* predict(int user);
	double* user_f(int user);
	double* item_f(int item);
	double mse(vector<pair<pair<int,int>,double>> events);
	double mae(vector<pair<pair<int,int>,double>> events);
	double rmse(vector<pair<pair<int,int>,double>> events);

};