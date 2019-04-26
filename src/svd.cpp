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

void SVD::fit(Dataset *ds, bool verbose=true){
	ds_ = ds;

	init_matrices(ds->n_users(),ds->n_items(),f_);
	randomize_matrices();

	generate_global_mean(ds->events());

	if(bias_){
		user_bias_.assign(n_users_,0.0);
		item_bias_.assign(n_items_,0.0);
	}

	double acc_error;

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

		if (verbose and !(it%(epochs_/10))){
			cerr << "Epoch #" << it << endl;
			cerr << "MSE: " << acc_error/ds->events().size() << endl;
		}
	}
	if(verbose){
		cerr << "Final" << endl;
		cerr << "MSE: " << acc_error/ds->events().size() << endl;
	}
}

double* SVD::user_f(int user){
	return U_[user];
}

double* SVD::item_f(int item){
	return V_[item];
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
