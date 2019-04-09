#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

class Dataset{
	private:
		void init_ratings(int r, int c);
		float **ratings_;
		map<string,int> users_encode_;
		map<string,int> items_encode_;
		vector<string> users_;
		vector<string> items_;
		int n_items_;
		int n_users_;

	public:
		Dataset(){

		}

		Dataset(string path){
			load_ratings(path);
		}

		~Dataset(){
			for (int i = 0; i < n_users_; i++)
			    delete [] ratings_[i];
			delete [] ratings_;
		}

		void load_ratings(string path);
		float get_rating(string user,string item);
		float* user_factor(int user);
		float* item_factor(int item);
};