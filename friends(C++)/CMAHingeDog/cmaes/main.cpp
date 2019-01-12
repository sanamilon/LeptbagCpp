#include <vector>
#include <limits>
#include "cmaes.hpp"
#include "cmaes.cpp"
//#include "objective_func.cpp"
#include "test_function.cpp"
#include "func_cmaes.cpp"
#include "export.cpp"


using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

int main(){

	int maxiter = 10000;
	int N = 30;


	std::function<double(vec)> func = rosenbrock;
	//std::function<double(vec)> func = rotated_ellipsoid;
	//std::function<double(vec)> func = sphere;
	cmaes es(
			func,
			vec::Zero(N), 1, 20
			);

	//initialize val for record
	vec meanf = vec::Zero(maxiter);
	vec sigma = vec(maxiter);
	mat D = mat::Zero(maxiter, N);
	mat diagC = mat::Zero(maxiter, N);

	int lastitr = 0;
	for(int itr=0; itr<maxiter; itr++){
		lastitr = itr;

		es.generateSample();
		es.evaluate();
		es.updateParam();

		meanf(itr) = es.func(es.mean);
		sigma(itr) = es.sigma;
		D.row(itr) = es.D.transpose();
		diagC.row(itr) = es.C.diagonal().transpose();

		if(isnan(meanf(itr))){
			std::cout<<"func(mean) goes nan"<<std::endl;
			break;
		}
		if((itr!=0)&&(meanf(itr)==0)){
			std::cout<<"func(mean) goes 0(at itr="<<lastitr<<")"<<std::endl;
			break;
		}
	}

	//std::cout<<meanf<<std::endl;
	meanf = meanf.block(0, 0, lastitr, 1);

	export_data("result/es_result_meanf.csv", meanf);
	export_data("result/es_result_sigma.csv", sigma);
	export_data("result/es_result_D.csv", D);
	export_data("result/es_result_diagC.csv", diagC);

}
