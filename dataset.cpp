#include "dataset.hpp"
#include "linalg.hpp"

void Dataset::init_ratings(int r, int c){
	ratings_ = new double*[r];
	for (int i=0; i<r; i++){
		ratings_[i] = new double[c];
		for (int j=0; j<c; j++){	
			ratings_[i][j] = 0 ;
		}
	}

	n_users_ = r;
	n_items_ = c;
}

void Dataset::load_ratings(string path, double train_test){
	ifstream input_file;
	string line;
	vector<pair<pair<int,int>,double>> events_;
	int n_users = 0;
	int n_items = 0;

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
			users_encode_[user] = n_users;
			users_.push_back(user);
			count_users_items[n_users++] = 0;
		}

		if(items_encode_.find(item) == items_encode_.end()){
			items_encode_[item] = n_items++;
			items_.push_back(item);
		}

		int item_i = items_encode_[item];
		int user_i = users_encode_[user];
		int frating = stof(rating);

		

		count_users_items[user_i]++;

		events_.push_back(make_pair(make_pair(user_i,item_i),frating));
	}

	init_ratings(n_users,n_items);

	for(auto p : events_){
		pair<int,int> ui = p.first;
		double rating = p.second;
		int user = ui.first, item = ui.second;

		double test = double_rand(0,1);
		if(train_test>test && count_users_items[user] > 3){
			test_.push_back(p);
			count_users_items[user]--;
		}else{
			train_.push_back(p);
			ratings_[user][item] = rating;	
		}
	}

	input_file.close();
}

double Dataset::get_rating(string user,string item){
	int ui = users_encode_[user];
	int ii = items_encode_[item];
	return ratings_[ui][ii];
}

double** Dataset::ratings(){
	return ratings_;
}

int Dataset::n_users(){
	return n_users_;
}

int Dataset::n_items(){
	return n_items_;
}

vector<pair<pair<int,int>,double>> Dataset::test(){
	return test_;
}

vector<pair<pair<int,int>,double>> Dataset::train(){
	return train_;
}