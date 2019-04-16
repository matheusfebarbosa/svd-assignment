#include "linalg.hpp"
#include "svd.hpp"

#include <cmath>
#include <iostream>
#include <random>

void SVD::init_matrices(int r, int c, int f){
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
}

void SVD::randomize(){

	default_random_engine generator;
	normal_distribution<double> distribution(1,0.5);

	for (int i=0; i<r_; i++){
		for (int j=0; j<f_; j++){
			U_[i][j] = distribution(generator);
		}
	}

	for (int i=0; i<c_; i++){
		for (int j=0; j<f_; j++){
			V_[i][j] = distribution(generator);
		}
	}
}

void SVD::fit(Dataset &ds){
	init_matrices(ds.n_users(),ds.n_items(),f_);
	randomize();

	vector<double> user_bias(ds.n_users(),0);
	vector<double> item_bias(ds.n_items(),0); 

	double acc_error;

	for(int it = 0; it < epochs_; it++){
		
		acc_error = 0;

		for(auto ev : ds.train()){
			pair<int,int> ui = ev.first;
			double real_rating = ev.second;
			int u = ui.first, i = ui.second;

			double error = real_rating - (predict(u,i) + user_bias[u] + item_bias[i]);
			acc_error += error * error;

			// update bias
			// user_bias[u] += 0.5 * lr_ * (error - reg_ * user_bias[u]);
			// item_bias[i] += 0.5 * lr_ * (error - reg_ * item_bias[i]);

			// regularization by vector norm
			// double u_norm = 0;
			// double i_norm = 0;
			// for (int k = 0; k < f_; k++){
			// 	u_norm += U_[u][k] * U_[u][k];
			// 	i_norm += V_[i][k] * V_[i][k];
			// }
			// u_norm = sqrt(u_norm);
			// i_norm = sqrt(i_norm)

			for (int k = 0; k < f_; k++){
				double uuk = U_[u][k];
				double vik = V_[i][k];
				U_[u][k] += 2 * lr_ * (error * vik - reg_ * uuk) ;
				V_[i][k] += 2 * lr_ * (error * uuk - reg_ * vik) ;
			}
		}

		if (!(it%(epochs_/10))){
			cerr << "Epoch #" << it << endl;
			cerr << "MSE: " << acc_error/ds.train().size() << endl;
		}
	}

	cerr << "Final" << endl;
	cerr << "MSE: " << acc_error/ds.train().size() << endl;
}

double SVD::mse(vector<pair<pair<int,int>,double>> events){
	double acc_error = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		double error = rating - predict(u,i);
		acc_error += error * error;
	}

	return acc_error/events.size();
}


double SVD::mae(vector<pair<pair<int,int>,double>> events){
	double acc_error = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		double error = rating - predict(u,i);
		acc_error += fabs(error);
	}

	return acc_error/events.size();
}

double SVD::rmse(vector<pair<pair<int,int>,double>> events){
	return sqrt(mse(events));
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