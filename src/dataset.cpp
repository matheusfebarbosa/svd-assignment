#include "dataset.hpp"
#include "linalg.hpp"

#include <fstream>
#include <iostream>
#include <random>


void Dataset::load_ratings(string path){
	ifstream input_file;
	string line;

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

	input_file.close();
}

int Dataset::n_users(){
	return n_users_;
}

int Dataset::n_items(){
	return n_items_;
}

int Dataset::encode_user(string user){
	if(users_encode_.find(user) == users_encode_.end()){
		return -1;
	}
	int ui = users_encode_[user];

	if(user != users_[ui]){
		cerr << "panic: user table" << endl;
		exit(1);
	}

	return ui;
}

int Dataset::encode_item(string item){
	if(items_encode_.find(item) == items_encode_.end()){
		return -1;
	}
	int ii = items_encode_[item];
	
	if(item != items_[ii]){
		cerr << "panic: item table" << endl;
		exit(1);
	}

	return ii;
}

string Dataset::get_item(int item){
	return items_[item];
}

string Dataset::get_user(int user){
	return users_[user];
}

vector<pair<pair<int,int>,double>> Dataset::events(){
	return events_;
}

void Dataset::add_event(pair<pair<string,string>,double> event){
	string user = event.first.first;
	string item = event.first.second;
	double rating = event.second;

	if(users_encode_.find(user) == users_encode_.end()){
		users_encode_[user] = n_users_++;
		users_.push_back(user);
	}

	if(items_encode_.find(item) == items_encode_.end()){
		items_encode_[item] = n_items_++;
		items_.push_back(item);
	}

	int item_i = items_encode_[item];
	int user_i = users_encode_[user];

	events_.push_back(make_pair(make_pair(user_i,item_i),rating));
}

vector<Dataset*> k_fold(Dataset *ds, unsigned int k){
	vector<Dataset*> folds;

	for(unsigned int i=0;i<k;i++){
		folds.push_back(new Dataset());
	}

	default_random_engine generator{42};
  	uniform_int_distribution<int> distribution(0,k-1);

	for(auto p : ds->events()){
		int u = p.first.first;
		int i = p.first.second;
		double rating = p.second;

		string user = ds->get_user(u);
		string item = ds->get_item(i);
		int fold = distribution(generator);
		
		folds[fold]->add_event(make_pair(make_pair(user,item),rating));
	}

	return folds;
}