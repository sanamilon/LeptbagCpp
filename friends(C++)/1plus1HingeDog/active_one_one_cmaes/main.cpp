#include <iostream>
#include "optimizer.hpp"
#include "test_function.hpp"
#include "export.hpp"

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

double getMedian(vec x){

	std::sort(x.data(), x.data() + x.size());
	double median;
	if(x.size()%2==0){
		median = x(x.size()/2);
	}else{
		median = ( x(x.size()/2) + x(x.size()/2+1) ) /2.0;
	}
	return median;
}

//return median of time steps necessary to converge under pow(10, convergeInto)
double EStest(int resetNum, optimizer es){

	static const int convergeInto = -10;

	std::cout<<"resetNum : "<<resetNum<<std::endl;

	vec result = vec::Zero(resetNum);

	for(int i=0; i<resetNum; i++){
		int count = 0;
		do{
			es.generateSample();
			es.evaluating();
			es.updateParam();
			count++;
		}while( count==1 || es.parentEval > pow(10.0, convergeInto));

		result(i) = (double)count;
		es.resetAllParam();
	}

	return getMedian(result);

}

int main(){

	int N=10;
	int maxitr = 6000;

	std::function<double(vec)> func = discus;
	func(vec::Zero(2)); //for showing the function name

	//optimizer(func, init_mean, init_sigma, active)
	optimizer es(
			func,
			vec::Zero(N),
			1.0,
			true
			//false
			);

	//get ratio of median of evaluation count in active/non-active
	/*
	int medO = 100;
	int count = EStest(medOf, es);
	std::cout<<"median of all count : "<<count<<std::endl;

	es.resetAllParam();
	es.active = true;
	count = EStest(medOf, es);
	std::cout<<"median of all count : "<<count<<std::endl;

	int c=0;
	do{
		es.generateSample();
		es.evaluating();
		es.updateParam();
		c++;
	}while( c==1 || es.parentEval > pow(10.0, -10));
	*/


	//see transition of meanf, sigma, D, diagC in constant number of iteration
	std::cout<<"maxitr : "<<maxitr<<std::endl;

	//for record
	vec meanf = vec::Zero(maxitr);
	vec sigma = vec::Zero(maxitr);
	mat D = mat::Zero(maxitr, N);
	mat diagC = mat::Zero(maxitr, N);

	int lastitr = 0;
	for(int i=0; i<maxitr; i++){

		es.generateSample();
		es.evaluating();
		es.updateParam();

		meanf(i) = es.parentEval;
		sigma(i) = es.sigma;

		mat C = es.A*es.A.transpose();
		Eigen::SelfAdjointEigenSolver<mat> eig(C);
		D.row(i) = eig.eigenvalues().transpose();
		diagC.row(i) = C.diagonal().transpose();

		lastitr++;

		if(es.parentEval < pow(10, -9)){
			break;
		}

	}

	//record
	meanf = meanf.block(0, 0, lastitr, 1);
	sigma = sigma.block(0, 0, lastitr, 1);
	D = D.block(0, 0, lastitr, N);
	diagC = diagC.block(0, 0, lastitr, N);

	export_data("result/es_result_meanf.csv", meanf);
	export_data("result/es_result_sigma.csv", sigma);
	export_data("result/es_result_D.csv", D);
	export_data("result/es_result_diagC.csv", diagC);


}
