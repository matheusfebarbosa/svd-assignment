#include "dataset.hpp"
#include "linalg.hpp"

#include <fstream>
#include <iostream>
#include <random>


void Dataset::load_ratings(string path, double train_test){
	ifstream input_file;
	string line;
	vector<pair<pair<int,int>,double>> events_;

	input_file.open(path);

	//discard header
	getline(input_file,line);

	map<int,int> count_users_items;

	while(getline(input_file,line)){
		int ii = line.find(":");
		int ri = line.find(",");
		int ti = line.find(",",ri+1);

		string user = line.substr(0,ii);
		string item = line.substr(ii+1,ri-ii-1);
		string rating = line.substr(ri+1,ti-ri-1);

		if(users_encode_.find(user) == users_encode_.end()){
			users_encode_[user] = n_users_;
			users_.push_back(user);
			count_users_items[n_users_++] = 0;
		}

		if(items_encode_.find(item) == items_encode_.end()){
			items_encode_[item] = n_items_++;
			items_.push_back(item);
		}

		int item_i = items_encode_[item];
		int user_i = users_encode_[user];
		int frating = stof(rating);

		count_users_items[user_i]++;

		events_.push_back(make_pair(make_pair(user_i,item_i),frating));
	}

	default_random_engine generator;
  	bernoulli_distribution distribution(train_test);

	for(auto p : events_){
		int user = p.first.first;
		
		if(distribution(generator) && count_users_items[user] > 3){
			test_.push_back(p);
			count_users_items[user]--;
		}else{
			train_.push_back(p);
		}
	}

	input_file.close();
}

int Dataset::n_users(){
	return n_users_;
}

int Dataset::n_items(){
	return n_items_;
}

int Dataset::encode_user(string user){
	return users_encode_[user];
}

int Dataset::encode_item(string item){
	return items_encode_[item];
}

vector<pair<pair<int,int>,double>> Dataset::test(){
	return test_;
}

vector<pair<pair<int,int>,double>> Dataset::train(){
	return train_;
}

double Dataset::global_mean(){
	if (global_mean_ == 0){
		for(auto ev : train_){
			global_mean_ += ev.second;
		}
		for(auto ev : test_){
			global_mean_ += ev.second;
		}

		global_mean_ /= ((float)train_.size() + test_.size());
	}

	return global_mean_;
}