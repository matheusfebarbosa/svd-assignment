#include "linalg.hpp"
#include "svd.hpp"


void SVD::init_ur(int r, int c, int f){
	U_ = new float*[r];
	for (int i=0; i<r; i++){
		U_[i] = new float[f];
	}

	V_ = new float*[c];
	for (int i=0; i<c; i++){
		V_[i] = new float[f];
	}

	r_ = r;
	c_ = c;
	f_ = f;
}

void SVD::randomize(){
	for (int i=0; i<r_; i++){
		for (int j=0; j<f_; j++){
			U_[i][j] = float_rand(-5,5);
		}
	}

	for (int i=0; i<c_; i++){
		for (int j=0; j<f_; j++){
			V_[i][j] = float_rand(-5,5);
		}
	}
}

void SVD::fit(Dataset ds, int n_factors, float lr, int epochs){
	fit(ds.ratings(), ds.n_users(), ds.n_items(), n_factors, lr, epochs);
}

void SVD::fit(float **matrix, int r, int c, int n_factors, float lr, int epochs){
	init_ur(r,c,n_factors);
	randomize();

	lr *= 2;

	vector<pair<int,int>> events;
	for (int i = 0; i < r; i++){
		for (int j = 0; j < c; j++){
			if(matrix[i][j] != 0.0){
				events.push_back(make_pair(i,j));
			}
		}
	}

	while(epochs--){
		cerr << "Interation " << epochs << endl;
		float sum = 0;

		for(auto ev : events){
			int i = ev.first, j = ev.second;
			float error = matrix[i][j] - predict(i,j);
			sum += error * error;

			error *= lr;

			for (int k = 0; k < f_; k++){
				U_[i][k] += error * V_[j][k];
			}

			for (int k = 0; k < f_; k++){
				V_[j][k] += error * U_[i][k];
			}
		}

		cout << "MSE: " << sum/events.size() << endl;
	}
}

float SVD::mse(vector<pair<pair<int,int>,float>> events){
	float sum = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		float rating = ev.second;

		float error = rating - predict(u,i);
		sum += error * error;
	}

	return sum/events.size();
}

float SVD::predict(int user, int item){
	return dot_product(U_[user],V_[item],f_);
}

float* SVD::user_f(int user){
	return U_[user];
}

float* SVD::item_f(int item){
	return V_[item];
}