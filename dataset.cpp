#include "dataset.hpp"

void Dataset::init_ratings(int r, int c){
	ratings_ = new float*[r];
	for (int i=0; i<r; i++){
		ratings_[i] = new float[c];
	}

	n_users_ = r;
	n_items_ = c;
}

void Dataset::load_ratings(string path){
	ifstream input_file;
	string line;
	vector<pair<pair<int,int>,float>> aux_data;
	int n_users = 0;
	int n_items = 0;

	input_file.open(path);

	//discard header
	getline(input_file,line);

	while(getline(input_file,line)){
		int ii = line.find(":");
		int ri = line.find(",");
		int ti = line.find(",",ri+1);

		string user = line.substr(0,ii);
		string item = line.substr(ii+1,ri-ii-1);
		string rating = line.substr(ri+1,ti-ri-1);

		if(users_encode_.find(user) == users_encode_.end()){
			users_encode_[user] = n_users++;
			users_.push_back(user);
		}

		if(items_encode_.find(item) == items_encode_.end()){
			items_encode_[item] = n_items++;
			items_.push_back(item);
		}

		int item_i = items_encode_[item];
		int user_i = users_encode_[user];
		int frating = stof(rating);

		aux_data.push_back(make_pair(make_pair(user_i,item_i),frating));
	}

	init_ratings(n_users,n_items);

	for(auto p : aux_data){
		pair<int,int> ui = p.first;
		float rating = p.second;
		ratings_[ui.first][ui.second] = rating;
	}

	input_file.close();
}

float Dataset::get_rating(string user,string item){
	int ui = users_encode_[user];
	int ii = items_encode_[item];
	return ratings_[ui][ii];
}

float* Dataset::user_factor(int user){
	return ratings_[user];
}

float* Dataset::item_factor(int item){
	return NULL;
}