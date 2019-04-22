#ifndef DATASET_H
#define DATASET_H 

#include <map>
#include <string>
#include <vector>

using namespace std;

class Dataset{

private:
	vector<pair<pair<int,int>,double>> train_;
	vector<pair<pair<int,int>,double>> test_;
	map<string,int> users_encode_;
	map<string,int> items_encode_;
	vector<string> users_;
	vector<string> items_;
	unsigned int n_items_;
	unsigned int n_users_;

	void init_ratings(int r, int c);

public:
	Dataset(){
		n_items_ = 0;
		n_users_ = 0;
	}

	Dataset(string path, double train_test) : Dataset() {
		load_ratings(path, train_test);
	}
	
	void load_ratings(string path, double train_test);
	int n_users();
	int n_items();
	int encode_user(string user);
	int encode_item(string item);
	vector<pair<pair<int,int>,double>> test();
	vector<pair<pair<int,int>,double>> train();

};

#endif