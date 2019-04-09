#include "linalg.hpp"

double dot_product(double *A, double *B, int n){
	double prod = 0;

	for (int i=0; i < n; i++){
		prod += A[i] * B[i];
	}

	return prod;
}

double double_rand(double min, double max){
    return ((double(rand()) / double(RAND_MAX)) * (max - min)) + min;
}