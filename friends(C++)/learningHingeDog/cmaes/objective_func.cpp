#ifndef OBJECTIVE_FUNC_CPP
#define OBJECTIVE_FUNC_CPP

#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <array>
#include <Eigen/Dense>


//exp( linspace[first, last] )
template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, 1> expList(float first, float last, int n)
{
	Eigen::Matrix<T, Eigen::Dynamic, 1> vector(n); // native C++ array or vector can be used of course
	float m = (float) 1 / (n - 1);
	float quotient = pow(last / first, m);

	vector(0) = first;

	for (int i = 1; i < n; i++) // DenseIndex is just a typedef ptrdiff_t from the Eigen library
		vector(i) = vector(i - 1) * quotient;

	return vector;
}

//gram_schmidt, each row is normalized base
template<typename T>
Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> gram_schmidt(Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic> mat){
	//if(debug) std::cout<<"gram_schmidt"<<std::endl;
	auto Ro = mat;
	for(int i=0; i<Ro.rows(); i++){
		for(int j=0; j<i; j++){
			Ro.row(i) = Ro.row(i) - Ro.row(i).dot(Ro.row(j)) * Ro.row(j);
		}
		Ro.row(i) /= Ro.row(i).norm();
	}
	return Ro;
}


static Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> R;
static bool fixed_rotation = false;
static bool R_initialized = false;
static const float EllipsoidCondition = 3;
//rotation of ellipsoid
//optimal(0, ..., 0)
template<typename type>
type rotated_ellipsoid(Eigen::Matrix<type, Eigen::Dynamic, 1> arx){

	if(fixed_rotation){ srand(7); }

	if(!R_initialized){
		R = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>::Random(arx.size(), arx.size());
		R = gram_schmidt<double>(R);
		R_initialized = true;
	}

	Eigen::Matrix<type, Eigen::Dynamic, 1> w = expList<type>(1.0, pow(10.0, EllipsoidCondition), arx.size());
	Eigen::Matrix<type, Eigen::Dynamic, 1> y;
	y = R.cast<type>()*arx;

	return sqrt( ( w.array()*(y.array()*y.array()) ).sum() );
}

//optimal(0, ..., 0)
template<typename type>
type sphere(Eigen::Matrix<type, Eigen::Dynamic, 1> arx){
	return arx.norm();
}


#endif
