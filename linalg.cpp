#include "linalg.hpp"

float dot_product(float *A, float *B, int n){
	float prod = 0;

	for (int i=0; i < n; i++){
		prod += A[i] * B[i];
	}

	return prod;
}

float float_rand(float min, float max){
    return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}