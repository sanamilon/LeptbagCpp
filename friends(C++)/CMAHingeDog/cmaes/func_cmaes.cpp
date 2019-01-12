#ifndef FUNC_CMAES_CPP
#define FUNC_CMAES_CPP

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <random>
#include <math.h>
#include <array>

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

std::vector<int> argsort(const vec &v){

	std::vector<int> idx;
	idx.resize(v.size());
	std::iota(idx.begin(), idx.end(), 0); 
	// sort indexes based on comparing values in v
	std::sort( idx.begin(), idx.end(),
			[&v](size_t i1, size_t i2) {return v[i1] < v[i2];} );

	return idx;
}

mat sort_matrix_row_with_indices(mat& m, std::vector<int> idx){

	mat sorted = mat(m.rows(), m.cols());
	for(int i=0; i<idx.size(); i++){
		sorted.row(i) = m.row(idx[i]);
	}

	return sorted;

};

#endif
