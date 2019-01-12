#ifndef EXPORT_CPP
#define EXPORT_CPP

#include <fstream>
#include <Eigen/Dense>

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

void export_data(std::string filename, mat m){

	std::ofstream data;
	data.open(filename);
	for(int i=0; i<m.rows(); i++){
		for(int j=0; j<m.cols(); j++){
			data<<m(i, j);
			if(j!=m.cols()-1){
				data<<",";
			}
		}
		data<<std::endl;
	}
	data.close();

};

void export_data(std::string filename, vec v){

	std::ofstream data;
	data.open(filename);
	for(int i=0; i<v.size(); i++){
		data<<v(i);
		if(i!=v.size()-1){
			data<<",";
		}
	}
	data<<std::endl;
	data.close();

};

#endif
