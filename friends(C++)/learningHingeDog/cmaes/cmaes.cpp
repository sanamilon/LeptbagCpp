#ifndef CMAES_CPP
#define CMAES_CPP

#include <limits>
#include <iostream>
#include <typeinfo>
#include <random>
#include <math.h>
#include <Eigen/Dense>
#include "func_es.cpp"


static const bool rank_mu = true;
static const bool rank_one = true;

template <typename T>
class cmaes{

	using vec = Eigen::Matrix<T, Eigen::Dynamic, 1>;
	using mat = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>;

	public:
		std::function<T(vec)> func; //evaluating function
		vec mean; //(N, 1)
		T sigma; //step size
		int N; //degree of the search space
		mat arz; //(nsample, N), generated from standard normal distribution
		mat ary; //(nsample, N), affined arz
		mat sample; //候補解, (nsample, N)
		vec arf; //各サンプルの評価値, (nsample, 1)
		vec D; //eigenvalue
		mat B; //eigenvector
		mat C; //generating distribution
		vec weights; //for selected sample on evaluation

		//for CSA
		vec ps; //evolution path
		T mueff; //mutation coeff
		T cs; //coeff for CSA
		T ds;
		T chiN; //expectation of standard normal distribution

		//forCMA
		vec pc; //evolution path for rank-one update
		T cc; //coeff for pc
		T cmu; //coeff for rank-mu
		T c1; //coeff for rank-one


		//constructor
		cmaes(	std::function<T(vec)> function,
				vec init_mean,
				T init_sigma,
				int nsample){

			std::cout<<"strategy : CSA";
			if(rank_mu){ std::cout<<" + rank-mu"; }
			if(rank_one){ std::cout<<" + rank-one"; }
			std::cout<<std::endl;
			std::cout<<"precision : "<<typeid(T).name()<<std::endl;
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
			double topN = 10.0;
			T w = topN / T(nsample);
			//sum of weights is 1. Top 1/topN goes next with those weights.
			this->weights.block(0, 0, int(nsample/topN), 1) = vec::Constant(int(nsample/topN), w);

			this->ps = vec::Zero(this->N);
			this->mueff = 1.0 / (this->weights.array() * this->weights.array()).sum();
			this->cs = (2.0 + this->mueff) / ( T(this->N)+3.0+this->mueff );
			this->ds = 1.0 + this->cs + fmax(0.0, sqrt(this->mueff/T(this->N)) - 1);
			this->chiN = sqrt(T(this->N))
				* (	1.0 
						- 1.0/(4.0*T(this->N))
						+ 1.0/(21.0*T(this->N*this->N))
				  );

			this->pc = vec::Zero(this->N);
			this->cc = 4.0/(T(this->N) + 4);
			this->cmu = this->mueff / (T(this->N*this->N)/2.0 + T(this->N) + this->mueff);
			this->c1 = 2.0/T(this->N*this->N);


		}


		void generateSample(){

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

		void evaluate(){
			for(int i=0; i<this->sample.rows(); i++){
				this->arf(i) = this->func(this->sample.row(i).transpose());
			}
		}


		//update ps, C, sigma, mean
		void updateParam(){

			std::vector<int> idx = argsort<T>(this->arf);

			mat sorted_arz = sort_matrix_row_with_indices<T>(this->arz, idx);
			//norm of evolution path is expected to be 1 when C is for standard normal distribution
			this->ps = (1.0 - this->cs)*this->ps
				+ sqrt(this->cs*(2.0-this->cs)*this->mueff)
				*(sorted_arz.transpose()*this->weights);

			mat sorted_ary = sort_matrix_row_with_indices<T>(this->ary, idx);


			T ccov = 0.0;
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

};

#endif
