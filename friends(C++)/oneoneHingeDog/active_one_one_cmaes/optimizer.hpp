#ifndef OPTIMIZER_HPP
#define OPTIMIZER_HPP

#include <iostream>
#include <deque>
#include <Eigen/Dense>

class optimizer{

	using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
	using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

	public:

		bool active;

		std::function<double(vec)> func; //evaluating function
		vec init_mean;
		double init_sigma;

		//not constant
		vec mean; //(N, 1)
		double sigma; //step size
		vec sample; //候補解, (nsample, N)
		vec arz; //(nsample, N), generated from standard normal distribution
		double parentEval;

		double Psucc;
		vec searchPath;

		mat A; //cholesky factor
		mat Ainv;

		std::deque<double> anceEval;

		mat C; //generating distribution
		vec D; //eigenvalue
		mat B; //eigenvector

		//constatnt
		int N; //degree of the search space

		double Ptarget;
		double d;
		double cp;
		double c;

		double ccov_plus;
		double ccov_minus;

		int ancestorSize;

		optimizer(std::function<double(vec)> function, vec init_mean, double init_sigma, bool active);
		void resetAllParam();
		void generateSample();
		void evaluating();
		void updateParam();

};

#endif
