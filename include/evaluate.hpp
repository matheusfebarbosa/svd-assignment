#ifndef EVALUATE_H
#define EVALUATE_H

#include "dataset.hpp"
#include "svd.hpp" 

vector<Dataset*> k_fold(Dataset *ds, unsigned int k);
double k_fold_evaluate(SVD *svd, Dataset *ds, int n_folds, bool verbose);

#endif