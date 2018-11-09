#ifndef FUNC_ES_CPP
#define FUNC_ES_CPP

#include <iostream>
#include <Eigen/Dense>
#include <vector>
#include <random>
#include <math.h>
#include <array>

template<typename T>
std::vector<int> argsort(const Eigen::Matrix<T, Eigen::Dynamic, 1> &v){

	std::vector<int> idx;
	idx.resize(v.size());
	std::iota(idx.begin(), idx.end(), 0); 
	// sort indexes based on comparing values in v
	std::sort( idx.begin(), idx.end(),
			[&v](size_t i1, size_t i2) {return v[i1] < v[i2];} );

	return idx;
}

template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> sort_matrix_row_with_indices(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& m, std::vector<int> idx){

	Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> sorted = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>(m.rows(), m.cols());
	for(int i=0; i<idx.size(); i++){
		sorted.row(i) = m.row(idx[i]);
	}

	return sorted;

};

#endif
