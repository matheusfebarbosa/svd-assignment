#include "linalg.hpp"
#include "svd.hpp"

#include <cmath>
#include <iostream>
#include <random>

void SVD::init_matrices(unsigned int r, unsigned int c, unsigned int f){
	U_ = new double*[r];
	for (unsigned int i=0; i<r; i++){
		U_[i] = new double[f];
	}

	V_ = new double*[c];
	for (unsigned int i=0; i<c; i++){
		V_[i] = new double[f];
	}

	n_users_ = r;
	n_items_ = c;
}

void SVD::randomize_matrices(){

	default_random_engine generator;
	normal_distribution<double> distribution(0,0.1);

	for (unsigned int i=0; i<n_users_; i++){
		for (unsigned int j=0; j<f_; j++){
			U_[i][j] = distribution(generator);
		}
	}

	for (unsigned int i=0; i<n_items_; i++){
		for (unsigned int j=0; j<f_; j++){
			V_[i][j] = distribution(generator);
		}
	}
}

void SVD::fit(Dataset &ds, bool bias){
	init_matrices(ds.n_users(),ds.n_items(),f_);
	randomize_matrices();

	generate_global_mean(ds.train());
	generate_user_bias(ds.train());
	generate_item_bias(ds.train());

	if(bias){
		bias_ = true;
	}

	double acc_error;

	for(int it = 0; it < epochs_; it++){
		
		acc_error = 0;

		for(auto ev : ds.train()){
			pair<int,int> ui = ev.first;
			double real_rating = ev.second;
			int u = ui.first, i = ui.second;

			double error = real_rating - predict(u,i);
			acc_error += error * error;

			for (unsigned int k = 0; k < f_; k++){
				double uuk = U_[u][k];
				double vik = V_[i][k];
				if(bias){
					U_[u][k] += lr_ * (error * vik - reg_ * (uuk + user_bias_[u])) ;
					V_[i][k] += lr_ * (error * uuk - reg_ * (vik + item_bias_[i])) ;
				}else{
					U_[u][k] += lr_ * (error * vik - reg_ * uuk) ;
					V_[i][k] += lr_ * (error * uuk - reg_ * vik) ;
				}
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

double* SVD::user_f(int user){
	return U_[user];
}

double* SVD::item_f(int item){
	return V_[item];
}


double SVD::predict(int user, int item){
	if(bias_){
		return user_bias_[user] + item_bias_[item] + global_mean_ + interaction(user,item);
	}else{
		return interaction(user,item);
	}
}

double SVD::fixed(int user, int item){
	return user_bias_[user] + item_bias_[item] + global_mean_;
}

double SVD::interaction(int user, int item){
	return dot_product(U_[user],V_[item],f_);
}

void SVD::generate_global_mean(vector<pair<pair<int,int>,double>> events){
	global_mean_ = 0;
	for(auto ev : events){
		global_mean_ += ev.second;
	}

	global_mean_ /= ((double)events.size());
	cerr << "Global mean: " << global_mean_ << endl;
}

void SVD::generate_item_bias(vector<pair<pair<int,int>,double>> events){
	if(item_bias_.size() < n_items_){
		item_bias_.resize(n_items_);
	}

	map<int,int> count_ratings;

	for(auto ev : events){
		int item = ev.first.second;
		int rating = ev.second;
		count_ratings[item]++;
		item_bias_[item] += rating - global_mean_;
	}

	for(auto i : count_ratings){
		item_bias_[i.first] /= i.second;
	}
}

void SVD::generate_user_bias(vector<pair<pair<int,int>,double>> events){
	if(user_bias_.size() < n_users_){
		user_bias_.resize(n_users_);
	}

	map<int,int> count_ratings;

	for(auto ev : events){
		int user = ev.first.first;
		int rating = ev.second;
		count_ratings[user]++;
		user_bias_[user] += rating - global_mean_;
	}

	for(auto i : count_ratings){
		user_bias_[i.first] /= i.second;
	}
}

bool SVD::is_biased(){
	return bias_;
}

double SVD::mse(vector<pair<pair<int,int>,double>> events){
	double acn_items_error = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		double error = rating - predict(u,i);
		acn_items_error += error * error;
	}

	return acn_items_error/events.size();
}


double SVD::mae(vector<pair<pair<int,int>,double>> events){
	double acn_items_error = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		double error = rating - predict(u,i);
		acn_items_error += fabs(error);
	}

	return acn_items_error/events.size();
}

double SVD::rmse(vector<pair<pair<int,int>,double>> events){
	return sqrt(mse(events));
}

void SVD::check_baseline(vector<pair<pair<int,int>,double>> events){
	double acn_items_error = 0;

	for(auto ev : events){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		double error = rating - fixed(u,i);
		acn_items_error += error * error;
	}

	cerr <<"RMSE: " << sqrt(acn_items_error/events.size()) << endl;
	cerr <<"MSE: " << acn_items_error/events.size() << endl;
}
