#include <limits>
#include <iostream>
#include <random>
#include <math.h>
#include <Eigen/Dense>
#include "cmaes.hpp"
#include "func_cmaes.cpp"


static const bool rank_mu = true;
static const bool rank_one = true;


using vec = Eigen::Matrix<double, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>;

cmaes::cmaes(	std::function<double(vec)> function,
		Eigen::MatrixXd init_mean,
		double init_sigma,
		int nsample){

	std::cout<<"strategy : CSA";
	if(rank_mu){ std::cout<<" + rank-mu"; }
	if(rank_one){ std::cout<<" + rank-one"; }
	std::cout<<std::endl;
	std::cout<<"precision : "<<"d"<<std::endl;
	std::cout<<"dimention : "<<init_mean.size()<<std::endl;
	std::cout<<"initial mean : "<<init_mean.transpose()<<std::endl;
	std::cout<<"initial sigma : "<<init_sigma<<std::endl;
	std::cout<<"num of sample : "<<nsample<<std::endl;

	this->func = function;
	this->mean = init_mean;
	this->sigma = init_sigma;
	this->N = this->mean.size();
	this->sample = mat::Zero(nsample, this->N);
	this->arz = mat(this->sample.rows(), this->sample.cols()); //(nsample, N)
	this->arf = vec(nsample);

	this->D = vec::Ones(this->N);
	this->B = mat::Identity(this->N, this->N);
	this->C = mat::Identity(this->N, this->N);

	this->weights = vec::Zero(nsample);
	double w = 4.0 / double(nsample);
	//sum of each weight is 1. Top 1/4 goes next with those weights.
	this->weights.block(0, 0, int(nsample/4), 1) = vec::Constant(int(nsample/4), w);

	this->ps = vec::Zero(this->N);
	this->mueff = 1.0 / (this->weights.array() * this->weights.array()).sum();
	this->cs = (2.0 + this->mueff) / ( double(this->N)+3.0+this->mueff );
	this->ds = 1.0 + this->cs + fmax(0.0, sqrt(this->mueff/double(this->N)) - 1);
	this->chiN = sqrt(double(this->N))
		* (	1.0 
				- 1.0/(4.0*double(this->N))
				+ 1.0/(21.0*double(this->N*this->N))
		  );

	this->pc = vec::Zero(this->N);
	this->cc = 4.0/(double(this->N) + 4);
	this->cmu = this->mueff / (double(this->N*this->N)/2.0 + double(this->N) + this->mueff);
	this->c1 = 2.0/double(this->N*this->N);


}


void cmaes::generateSample(){

	std::random_device seed_gen;
	std::default_random_engine engine(seed_gen());
	std::normal_distribution<> dist(0.0, 1.0);

	for(int i=0; i<this->arz.rows(); i++){
		for(int j=0; j<this->arz.cols(); j++){
			this->arz(i, j) = dist(engine);
		}
	}

	this->ary = this->arz * this->B*this->D.cwiseSqrt().asDiagonal()*this->B.transpose();
	this->sample = mat::Ones(this->sample.rows(), this->sample.cols())*this->mean.asDiagonal()
		+ this->sigma*this->ary;

}

void cmaes::evaluate(){
	for(int i=0; i<this->sample.rows(); i++){
		this->arf(i) = this->func(this->sample.row(i).transpose());
	}
}


//update ps, C, sigma, mean
void cmaes::updateParam(){

	std::vector<int> idx = argsort(this->arf);

	mat sorted_arz = sort_matrix_row_with_indices(this->arz, idx);
	//norm of evolution path is expected to be 1 when C is for standard normal distribution
	this->ps = (1.0 - this->cs)*this->ps
		+ sqrt(this->cs*(2.0-this->cs)*this->mueff)
		*(sorted_arz.transpose()*this->weights);

	mat sorted_ary = sort_matrix_row_with_indices(this->ary, idx);


	double ccov = 0.0;
	if(rank_one){
		ccov += this->c1;
		this->pc = (1-this->cc)*this->pc + sqrt(this->cc*(2.0-this->cc)*this->mueff)
			*(sorted_ary.transpose()*this->weights);
	}
	if(rank_mu){ ccov += this->cmu*this->weights.sum(); }

	this->C = (1.0 - ccov)*this->C;
	if(rank_one){
		this->C += this->c1 * this->pc*this->pc.transpose();
	}
	if(rank_mu){
		this->C += this->cmu * (sorted_ary.transpose()*this->weights.asDiagonal()*sorted_ary);
	}


	Eigen::SelfAdjointEigenSolver<mat> eig(this->C);
	this->D = eig.eigenvalues();
	this->D = this->D.cwiseAbs();
	this->B = eig.eigenvectors();
	this->C=this->B*this->D.asDiagonal()*this->B.transpose();

	this->mean += this->sigma*sorted_ary.transpose() * this->weights;
	this->sigma *= exp( this->cs/this->ds*(this->ps.norm()/this->chiN-1.0) );


}

