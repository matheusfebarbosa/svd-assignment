#include "linalg.hpp"
#include "svd.hpp"

#include <cmath>
#include <iostream>
#include <random>

void SVD::init_matrices(unsigned int r, unsigned int c, unsigned int f){
	if(U_!=NULL){
		for (unsigned int i=0; i<n_users_; i++){
			delete U_[i];
		}
	}
	if(V_!=NULL){
		for (unsigned int i=0; i<n_items_; i++){
			delete V_[i];
		}
	}

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
	normal_distribution<double> distribution(dist_mean_,dist_deviation_);

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

void SVD::fit(Dataset *ds){
	ds_ = ds;

	init_matrices(ds->n_users(),ds->n_items(),f_);
	randomize_matrices();

	generate_global_mean(ds->events());

	if(bias_){
		user_bias_.assign(n_users_,0.0);
		item_bias_.assign(n_items_,0.0);
	}

	double acc_error;

	if(verbose_){
		cerr <<"-----------------------------" << endl;
		cerr <<"Started training..." << endl;
	}

	for(int it = 0; it < epochs_; it++){
		
		acc_error = 0;

		for(auto ev : ds->events()){
			pair<int,int> ui = ev.first;
			double real_rating = ev.second;
			int u = ui.first, i = ui.second;

			double error = real_rating - predict(u,i);
			acc_error += error * error;

			if(bias_){
				user_bias_[u] += lr_b_ * (error - reg_ * user_bias_[u]);
				item_bias_[i] += lr_b_ * (error - reg_ * item_bias_[i]);
			}

			for (unsigned int k = 0; k < f_; k++){
				double uuk = U_[u][k];
				double vik = V_[i][k];
				U_[u][k] += lr_m_ * (error * vik - reg_ * uuk) ;
				V_[i][k] += lr_m_ * (error * uuk - reg_ * vik) ;
			}
		}

		if (verbose_ and !(it%(epochs_/10))){
			cerr << "Epoch #" << it << endl;
			cerr << "MSE: " << acc_error/ds->events().size() << endl;
		}
	}
	if(verbose_){
		cerr << "Final" << endl;
		cerr << "MSE: " << acc_error/ds->events().size() << endl;
		cerr <<"-----------------------------" << endl;
	}
}

double SVD::predict(int user, int item){

	double prediction = global_mean_;

	if(bias_){
		if(user != -1){
			prediction += user_bias_[user];
		}

		if(item != -1){
			prediction += item_bias_[item];
		}

		if(user != -1 and item != -1){
			prediction += interaction(user,item);
		}
	}else{
		if(user != -1 and item != -1){
			prediction = interaction(user,item);
		}
	}

	return prediction;
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
}

double SVD::mse(Dataset *ds){
	double acn_items_error = 0;

	for(auto ev : ds->events()){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		int user = ds_->encode_user(ds->get_user(u));
		int item = ds_->encode_item(ds->get_item(i));

		double error = rating - predict(user,item);
		acn_items_error += error * error;
	}

	return acn_items_error/ds->events().size();
}

double SVD::mae(Dataset *ds){
	double acn_items_error = 0;

	for(auto ev : ds->events()){
		pair<int,int> ui = ev.first;
		int u = ui.first, i = ui.second;
		double rating = ev.second;

		int user = ds_->encode_user(ds->get_user(u));
		int item = ds_->encode_item(ds->get_item(i));

		double error = rating - predict(user,item);
		acn_items_error += fabs(error);
	}

	return acn_items_error/ds->events().size();
}

double SVD::rmse(Dataset *ds){
	return sqrt(mse(ds));
}