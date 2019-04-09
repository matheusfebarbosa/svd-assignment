#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Dataset{

private:
	float **ratings_;
	vector<pair<pair<int,int>,float>> train_;
	vector<pair<pair<int,int>,float>> test_;
	map<string,int> users_encode_;
	map<string,int> items_encode_;
	vector<string> users_;
	vector<string> items_;
	int n_items_;
	int n_users_;


	void init_ratings(int r, int c);

public:
	Dataset(){

	}

	Dataset(string path, float train_test){
		load_ratings(path, train_test);
	}

	~Dataset(){
		// for (int i = 0; i < n_users_; i++){
		//     delete [] ratings_[i];
		// }
		//delete [] ratings_;
	}

	void load_ratings(string path, float train_test);
	float get_rating(string user,string item);
	float* user_factor(int user);
	float* item_factor(int item);
	float** ratings();
	int n_users();
	int n_items();
	vector<pair<pair<int,int>,float>> test();
	vector<pair<pair<int,int>,float>> train();

};