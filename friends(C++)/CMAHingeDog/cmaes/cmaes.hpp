#pragma once

#include <Eigen/Dense>

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;
class cmaes{

	public:
		std::function<double(Eigen::MatrixXd)> func; //evaluating function
		Eigen::VectorXd mean; //(N, 1)
		double sigma; //step size
		int N; //degree of the search space
		Eigen::MatrixXd arz; //(nsample, N), generated from standard normal distribution
		Eigen::MatrixXd ary; //(nsample, N), affined arz
		Eigen::MatrixXd sample; //候補解, (nsample, N)
		Eigen::VectorXd arf; //各サンプルの評価値, (nsample, 1)
		Eigen::VectorXd D; //eigenvalue
		Eigen::MatrixXd B; //eigenvector
		Eigen::MatrixXd C; //generating distribution

		Eigen::VectorXd weights;
		
		//for CSA
		Eigen::VectorXd ps; //evolution path
		double mueff; //mutation coeff
		double cs; //coeff for CSA
		double ds;
		double chiN;

		//forCMA
		Eigen::VectorXd pc; //evolution path for rank-one update
		double cc; //coeff for pc
		double cmu; //coeff for rank-mu
		double c1; //coeff for rank-one


		cmaes(	std::function<double(vec)> function,
				Eigen::MatrixXd init_mean,
				double init_sigma,
				int nsample);

		void generateSample();
		void evaluate();
		void updateParam();

};
