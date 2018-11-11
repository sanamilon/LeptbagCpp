#ifndef EXPORT_CPP
#define EXPORT_CPP

#include <fstream>
#include <Eigen/Dense>

void export_data(std::string filename, Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> mat);
void export_data(std::string filename, Eigen::Matrix<double, Eigen::Dynamic, 1> vec);

#endif
