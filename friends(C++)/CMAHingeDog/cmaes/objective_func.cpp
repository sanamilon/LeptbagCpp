#ifndef OBFECTIVE_FUNC_CPP
#define OBFECTIVE_FUNC_CPP

#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <array>
#include <Eigen/Dense>

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;


//exp( linspace[first, last] )
vec expList(double first, double last, int n)
{
	vec vector(n); // native C++ array or vector can be used of course
	double m = (double) 1 / (n - 1);
	double quotient = pow(last / first, m);

	vector(0) = first;

	for (int i = 1; i < n; i++) // DenseIndex is just a typedef ptrdiff_t from the Eigen library
		vector(i) = vector(i - 1) * quotient;

	return vector;
}

//gram_schmidt, each row is normalized base
mat gram_schmidt(mat m){
	//if(debug) std::cout<<"gram_schmidt"<<std::endl;
	auto Ro = m;
	for(int i=0; i<Ro.rows(); i++){
		for(int j=0; j<i; j++){
			Ro.row(i) = Ro.row(i) - Ro.row(i).dot(Ro.row(j)) * Ro.row(j);
		}
		Ro.row(i) /= Ro.row(i).norm();
	}
	return Ro;
}



//rotation of ellipsoid
//optimal(0, ..., 0)
double rotated_ellipsoid(vec arx){

	static mat R;
	static bool fixed_rotation = false;
	static bool R_initialized = false;
	static const double EllipsoidCondition = 10;

	if(fixed_rotation){ srand(7); }

	if(!R_initialized){
		R = mat::Random(arx.size(), arx.size());
		R = gram_schmidt(R);
		R_initialized = true;
	}

	vec w = expList(1.0, pow(10.0, EllipsoidCondition), arx.size());
	vec y = R*arx;

	return sqrt( ( w.array()*(y.array()*y.array()) ).sum() );
}

//optimal(0, ..., 0)
double sphere(vec arx){
	return arx.norm();
}


#endif
