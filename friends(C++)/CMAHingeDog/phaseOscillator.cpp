#include <cmath>
#include <random>
#include <iostream>
#include <Eigen/Dense>

class phaseOscillator{
	
	public:

	int numOfOsci;
	double* theta;

	int degreeOfFourier;
	Eigen::Matrix<float, Eigen::Dynamic, 1> coeff;

	phaseOscillator(int numOfOsci, int degreeOfFourier){


		this->numOfOsci = numOfOsci;
		this->theta = new double[numOfOsci];
		for(int t=0; t<numOfOsci; t++){
			this->theta[t] = 0.0;
		}

		this->degreeOfFourier = degreeOfFourier;
		int coeffSize = numOfOsci + 2*degreeOfFourier*numOfOsci*(numOfOsci-1);
		this->coeff = Eigen::Matrix<float, Eigen::Dynamic, 1>(coeffSize);

		std::random_device seed_gen;
		std::default_random_engine engine(seed_gen());
		std::uniform_real_distribution<double> dist(-0.1, 0.1);

		for(int c=0; c<coeffSize; c++){
			this->coeff[c] = dist(engine);
		}

	}

	void calTheta(){

		int c=0;
		for(int i=0; i<this->numOfOsci; i++){

			this->theta[i] += this->coeff[c++];

			for(int j=0; j<this->numOfOsci; j++){
				if(i!=j){
					for(int k=0; k<this->degreeOfFourier; k++){
						this->theta[i] += this->coeff[c++]*sin(this->theta[i] - this->theta[j]);
						this->theta[i] += this->coeff[c++]*cos(this->theta[i] - this->theta[j]);
					}
				}
			}

			this->theta[i] += 12.0*M_PI;
			this->theta[i] = std::fmod(theta[i], 2.0*M_PI);
		}


	}


};
