#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <array>
#include <Eigen/Dense>

#include "test_function.hpp"

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

static int condition = 6;

//tools/////////////////////////////////////////////////////

//exp( linspace[first, last] )
vec expList(double first, double last, int n){
	vec vector(n); // native C++ array or vector can be used of course
	double m = (double) 1 / (n - 1);
	double quotient = pow(last / first, m);

	vector(0) = first;

	for (int i = 1; i < n; i++) // DenseIndex is just a typedef ptrdiff_t from the Eigen library
		vector(i) = vector(i - 1) * quotient;

	return vector;
}

//gram_schmidt, each row is normalized base
mat gram_schmidt(mat M){
	//if(debug) std::cout<<"gram_schmidt"<<std::endl;
	auto Ro = M;
	for(int i=0; i<Ro.rows(); i++){
		for(int j=0; j<i; j++){
			Ro.row(i) = Ro.row(i) - Ro.row(i).dot(Ro.row(j)) * Ro.row(j);
		}
		Ro.row(i) /= Ro.row(i).norm();
	}
	return Ro;
}

//test function//////////////////////////////////////////

//optimal(0, ..., 0)
double sphere(vec arx){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}

	return arx.norm();
}

//ellipsoid
//optimal(0, ..., 0)
double ellipsoid(vec arx){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}

	static int EllipsoidCondition = condition;
	static vec scaling;
	static bool scaling_initialized;

	if(!scaling_initialized){
		scaling = expList(1.0, pow(10.0, EllipsoidCondition), arx.size());
	}

	return sqrt( ( scaling.array()*(arx.array()*arx.array()) ).sum() );
}

//rotation of ellipsoid
//optimal(0, ..., 0)
double rotated_ellipsoid(vec arx){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}

	static int EllipsoidCondition = condition;
	static mat R;
	static bool fixed_rotation = false;
	static bool R_initialized = false;
	static vec w;

	if(fixed_rotation){ srand(7); }

	if(!R_initialized){
		R = mat::Random(arx.size(), arx.size());
		R = gram_schmidt(R);
		R_initialized = true;
		w = expList(1.0, pow(10.0, EllipsoidCondition), arx.size());
	}

	vec y;
	y = R*arx;

	return sqrt( ( w.array()*(y.array()*y.array()) ).sum() );
}

double different_powers(vec x){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}

	double eval = 0.0;
	for(int i=0; i<x.size(); i++){
		eval += pow( std::abs(x(i)), 2.0+10.0*i/(x.size()-1) );
	}
	return eval;
}

double rosenbrock(vec x){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}
	double eval = 0.0;
	for(int i=0; i<x.size()-1; i++){
		eval += 100.0*pow(x(i)*x(i) - x(i+1)*x(i+1), 2) + pow(x(i)-1, 2);
	}
	return eval;
}

double discus(vec x){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}
	double eval = 0.0;
	eval += pow(10.0, condition) * x(0)*x(0);
	for(int i=1; i<x.size(); i++){
		eval += x(i)*x(i);
	}
	return eval;
}

double cigar(vec x){
	static bool firstCall = true;
	if(firstCall){
		std::cout<<"function :"<<__FUNCTION__<<std::endl;
		firstCall = false;
	}
	double eval = 0.0;
	eval += x(0)*x(0);
	for(int i=1; i<x.size(); i++){
		eval += pow(10.0, condition) *x(i)*x(i);
	}
	return eval;
}
