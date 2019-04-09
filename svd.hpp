#include "dataset.hpp"

using namespace std;

class SVD{

private:
	float **U_;
	float **V_;
	float lr_;
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

	void fit(Dataset ds, int n_factors, float lr, int epochs);
	void fit(float **matrix, int r, int c, int n_factors, float lr, int epochs);
	float predict(int user, int factor);
	float* predict(int user);
	float* user_f(int user);
	float* item_f(int item);
	float mse(vector<pair<pair<int,int>,float>> events);

};