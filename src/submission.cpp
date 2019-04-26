#include <iostream>
#include <fstream>

#include "submission.hpp"

void generate_submission(SVD *svd, Dataset ds, string path){
	ifstream input_file;
	string line;

	input_file.open(path);

	//discard header
	getline(input_file,line);

	cout << line << "," << "Prediction" << endl;

	while(getline(input_file,line)){
		int ii = line.find(":");

		string user = line.substr(0,ii);
		string item = line.substr(ii+1);

		int i_user = ds.encode_user(user);
		int i_item = ds.encode_item(item);

		double pred_rating = svd->predict(i_user,i_item);

		if (pred_rating > 10){
			pred_rating = 10;
		}else if (pred_rating < 0){
			pred_rating = 0;
		}

		cout << user << ":" << item << "," << pred_rating << endl;
	}
}