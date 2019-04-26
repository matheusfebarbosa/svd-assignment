#ifndef DATASET_H
#define DATASET_H 

#include <map>
#include <string>
#include <vector>

using namespace std;

class Dataset{

private:
	vector<pair<pair<int,int>,double>> events_;
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

	Dataset(string path) : Dataset() {
		load_ratings(path);
	}
	
	void load_ratings(string path);
	int n_users();
	int n_items();
	int encode_user(string user);
	int encode_item(string item);
	string get_user(int user);
	string get_item(int item);
	vector<pair<pair<int,int>,double>> events();
	void add_event(pair<pair<string,string>,double> event);
};

//utils
vector<Dataset*> k_fold(Dataset *ds, unsigned int k);

#endif