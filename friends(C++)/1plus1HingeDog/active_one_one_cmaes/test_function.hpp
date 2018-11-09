#ifndef OBJECTIVE_FUNC_CPP
#define OBJECTIVE_FUNC_CPP

#include <iostream>
#include <vector>
#include <random>
#include <math.h>
#include <array>
#include <Eigen/Dense>

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;


//tools/////////////////////////////////////////////////////

//exp( linspace[first, last] )
vec expList(double first, double last, int n);

//gram_schmidt, each row is normalized base
mat gram_schmidt(mat M);

//test function//////////////////////////////////////////

//optimal(0, ..., 0)
double sphere(vec x);
double ellipsoid(vec x);
double rotated_ellipsoid(vec x);
double different_powers(vec x);
double rosenbrock(vec x);
double discus(vec x);
double cigar(vec x);

#endif
