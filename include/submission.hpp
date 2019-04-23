#ifndef SUBMISSION_H
#define SUBMISSION_H 

#include "svd.hpp"
#include "dataset.hpp"

#include <string>

void generate_submission(SVD *svd, Dataset ds, string path);

#endif