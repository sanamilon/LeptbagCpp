#include <vector>
#include <limits>
#include "cmaes.cpp"
#include "objective_func.cpp"
#include "func_es.cpp"
#include "export.cpp"

//static const bool debug = false;

int main(){

	using precision = float;
	using vec = Eigen::Matrix<precision, Eigen::Dynamic, 1>;
	using mat = Eigen::Matrix<precision, Eigen::Dynamic, Eigen::Dynamic>;

	int maxiter = 5000;
	int N = 50;


	std::function<precision(vec)> func = rotated_ellipsoid<precision>;
	cmaes<precision> es(
			func,
			vec::Zero(N), 1, 10
			);

	//initialize val for record
	vec meanf = vec::Zero(maxiter);
	vec sigmaN = vec(maxiter);
	mat D = mat::Zero(maxiter, N);
	mat diagC = mat::Zero(maxiter, N);

	//learning
	int lastitr = 0;
	for(int itr=0; itr<maxiter; itr++){
		lastitr = itr;

		es.generateSample();
		es.evaluate();
		es.updateParam();

		//record
		meanf(itr) = es.func(es.mean);
		sigmaN(itr) = es.sigma*es.N;
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

	export_data<precision>("result/es_result_meanf.csv", meanf);
	export_data<precision>("result/es_result_sigmaN.csv", sigmaN);
	export_data<precision>("result/es_result_D.csv", D);
	export_data<precision>("result/es_result_diagC.csv", diagC);

}
