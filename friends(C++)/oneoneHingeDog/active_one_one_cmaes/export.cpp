#include "export.hpp"

void export_data(std::string filename, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> mat){

	std::ofstream data;
	data.open(filename);
	for(int i=0; i<mat.rows(); i++){
		for(int j=0; j<mat.cols(); j++){
			data<<mat(i, j);
			if(j!=mat.cols()-1){
				data<<",";
			}
		}
		data<<std::endl;
	}
	data.close();

};

void export_data(std::string filename, Eigen::Matrix<double, Eigen::Dynamic, 1> vec){

	std::ofstream data;
	data.open(filename);
	for(int i=0; i<vec.size(); i++){
		data<<vec(i);
		if(i!=vec.size()-1){
			data<<",";
		}
	}
	data<<std::endl;
	data.close();

};
