#include "linalg.hpp"
#include "svd.hpp"


void SVD::init_ur(int r, int c, int f){
	U_ = new double*[r];
	for (int i=0; i<r; i++){
		U_[i] = new double[f];
	}

	V_ = new double*[c];
	for (int i=0; i<c; i++){
		V_[i] = new double[f];
	}

	r_ = r;
	c_ = c;
	f_ = f;
}

void SVD::randomize(){
	for (int i=0; i<r_; i++){
		for (int j=0; j<f_; j++){
			U_[i][j] = double_rand(-5,5);
		}
	}

	for (int i=0; i<c_; i++){
		for (int j=0; j<f_; j++){
			V_[i][j] = double_rand(-5,5);
		}
	}
}

void SVD::fit(Dataset ds, int n_factors, double lr, int epochs){
	fit(ds.ratings(), ds.n_users(), ds.n_items(), n_factors, lr, epochs);
}

void SVD::fit(double **matrix, int r, int c, int n_factors, double lr, int epochs){
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
		double sum = 0;

		for(auto ev : events){
			int i = ev.first, j = ev.second;
			double error = matrix[i][j] - predict(i,j);
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

double SVD::mse(vector<pair<pair<int,int>,double>> events){
	double sum = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		double error = rating - predict(u,i);
		sum += error * error;
	}

	return sum/events.size();
}

double SVD::predict(int user, int item){
	return dot_product(U_[user],V_[item],f_);
}

double* SVD::user_f(int user){
	return U_[user];
}

double* SVD::item_f(int item){
	return V_[item];
}