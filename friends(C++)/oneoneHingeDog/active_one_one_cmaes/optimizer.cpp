#include <random>
#include <math.h>
#include "optimizer.hpp"

using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;


optimizer::optimizer(
		std::function<double(vec)> function,
		vec init_mean,
		double init_sigma,
		bool active
		){

	//actively optimizing for decrease harmful dimention
	this->active = active;

	std::cout<<"strategy : ";
	if(this->active){ std::cout<<"active "; }
	else{ std::cout<<"non-active "; }
	std::cout<<"(1+1)-CMA-ES"<<std::endl;

	std::cout<<"precision : d"<<std::endl;
	std::cout<<"dimention : "<<init_mean.size()<<std::endl;
	std::cout<<"initial mean : "<<init_mean.transpose()<<std::endl;
	std::cout<<"initial sigma : "<<init_sigma<<std::endl;

	this->func = function;
	this->init_mean = init_mean;
	this->init_sigma = init_sigma;

	//constant
	this->N = init_mean.size();

	this->Ptarget = pow(10, -3);//2.0 / 11.0;
	this->d = 1.0 + this->N/2.0;
	this->cp = 1.0/12.0;
	this->c = 2.0 / (this->N + 2.0);

	this->ccov_plus = 2.0 / (this->N*this->N + 6);
	this->ccov_minus = 0.4 / ( pow(this->N, 1.6) + 1 );

	this->ancestorSize = 5;

	//not constant
	this->mean = init_mean;
	this->sigma = init_sigma;
	this->sample = vec::Zero(this->N, 1);
	this->arz = vec::Zero(this->N, 1);

	this->Psucc = 0.0;
	this->searchPath = vec::Zero(this->N, 1);

	this->A = mat::Identity(this->N, this->N);
	this->Ainv = mat::Identity(this->N, this->N);

	//for analyzation, originally not necessary
	this->C = mat::Identity(this->N, this->N);
	this->D = vec::Ones(this->N);
	this->B = mat::Identity(this->N, this->N);


}

void optimizer::resetAllParam(){

	this->mean = this->init_mean;
	this->sigma = this->init_sigma;
	this->sample = vec::Zero(this->N, 1);
	this->arz = vec::Zero(this->N, 1);
	this->parentEval = this->func(this->init_mean);

	this->Psucc = 0.0;
	this->searchPath = vec::Zero(this->N, 1);
	this->A = mat::Identity(this->N, this->N);
	this->Ainv = mat::Identity(this->N, this->N);

	this->C = mat::Identity(this->N, this->N);
	this->D = vec::Ones(this->N);
	this->B = mat::Identity(this->N, this->N);

	while(this->anceEval.size()!=0){
		this->anceEval.pop_back();
	}

}

void optimizer::generateSample(){

	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::normal_distribution<> dist(0.0, 1.0);
	for(int i=0; i<this->N; i++){
		this->arz(i) = dist(engine);
	}

	this->sample = this->mean + this->sigma * this->A * this->arz;
}

void optimizer::evaluating(){
	this->anceEval.push_front(func(this->sample));
	if(this->anceEval.size() > this->ancestorSize+1){
		this->anceEval.pop_back();
	}
}

void optimizer::updateParam(){

	//for first itr
	if(this->anceEval.size()==1){ this->parentEval = this->anceEval.at(0); }

	//non-active (1+1)
	if(this->parentEval >= this->anceEval.at(0)){

		this->mean = this->sample;

		this->Psucc = (1.0 - this->cp)*this->Psucc + this->cp;
		this->searchPath = (1.0 - this->c)*this->searchPath
							+ sqrt( this->c*(2.0-this->c) )*this->A*this->arz;


		double coeffa = sqrt(1.0 - this->ccov_plus);

		vec w = this->Ainv * this->searchPath;
		double coeffb = coeffa 
					/ w.squaredNorm()
					* (
						sqrt( 1.0 + this->ccov_plus/(1.0-this->ccov_plus)*w.squaredNorm() )
						- 1.0
						);

		this->A = coeffa*this->A + coeffb*this->A*w*w.transpose();
		this->Ainv = this->Ainv / coeffa
						- coeffb
						/(	coeffa*coeffa
							+ coeffa*coeffb*w.squaredNorm()
							)
						*( w*w.transpose()*this->Ainv );


		this->parentEval = this->anceEval.at(0);
	}else{
		this->Psucc = (1.0 - this->cp)*this->Psucc;
	}

	this->sigma = this->sigma * exp( ( this->Psucc - this->Ptarget )
									/ ( 1.0 - this->Ptarget )
									/ this->d
									);


	//active : shaving covariance matrix
	if(optimizer::active){

		bool inferior = true;
		for(int i=1; i<this->anceEval.size(); i++){
			if( this->anceEval.at(0) <= this->anceEval.at(i) ){
				inferior = false;
				break;
			}
		}

		if(inferior){

			double cov = this->ccov_minus;
			if(
					1.0<
					cov*(2.0*this->arz.squaredNorm() - 1.0)
			  ){
				cov = 1.0 / (2.0+this->arz.squaredNorm() - 1.0);
			}

			double coeffa = sqrt(1.0 + cov);
			double coeffb = coeffa 
				/ this->arz.squaredNorm()
				* (	sqrt( 1.0 - cov/(1.0+cov)*this->arz.squaredNorm() )
					- 1.0
				  );


			vec w = this->arz;//this->Ainv * this->searchPath;
			this->A = coeffa*this->A + coeffb*this->A*w*w.transpose();
			this->Ainv = this->Ainv / coeffa
				- coeffb
				/(	coeffa*coeffa
						+coeffa*coeffb*w.squaredNorm()
				 )
				*( w*w.transpose()*this->Ainv );

		}

	}

}
