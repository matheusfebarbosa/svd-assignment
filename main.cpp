#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include "dataset.hpp"

using namespace std;

int main(){

	Dataset ds("./dataset/ratings.csv");

	cout << ds.get_rating("u0026502","i0444778");
	cout << ds.get_rating("u0024257","i0338013");

	return 0;
}