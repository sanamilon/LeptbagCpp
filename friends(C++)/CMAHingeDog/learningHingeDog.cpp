#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include <chrono>
#include "cmaes/cmaes.cpp"
#include "cmaes/objective_func.cpp"
#include "cmaes/func_cmaes.cpp"
#include "cmaes/export.cpp"
#include "phaseOscillator.cpp"
#include "../../source/japarilib.hpp"

template <typename... ARGS>
parameterPack* paramPack(ARGS... args){
	return new parameterPack(sizeof... (ARGS), args...);
}



const bool phaseOsci = true;
const int POdegreeOfFourier = 1;
const int numofdog = 50;
const int dnacol = 20;
const int dnarow = 4;

class dog {

	public:

	int tag = 0;

	Eigen::Vector3f initPosition;

	float dna [20][4] = {
		{ -0.74438, -0.0280514,  -0.237172,  0.612237},
		{ 0.512584,   -0.32883,  -0.845608, -0.138221},
		{ 0.418906,   -1.33678,    1.37841,   1.42834},
		{-0.832418, -0.0840262,   0.833291, -0.134319},
		{ -1.26931,   0.726787, -0.0124514, -0.314659},
		{-0.861292,   0.348597,   0.405946,  -1.06412},
		{ 0.311971,  0.0144064,   0.329286, -0.333543},
		{ 0.936644,  -0.988285,  -0.450094, -0.718639},
		{ -1.47339,  -0.423863,  0.0677283,  0.440922},
		{ -1.06763,   -1.39734,   -0.70326,   -0.5131},
		{ 0.444998,   0.245339,    1.23144,  -1.28619},
		{ 0.867622,   0.464909,   -1.08054,  -0.46792},
		{  0.73467,   0.637727,   -1.10756,  0.303961},
		{-0.112369,  -0.839047,   0.822885,  -1.19256},
		{-0.995347,  -0.271012,  -0.809764,  0.915184},
		{ 0.333942,   -1.25962, -0.0206624,  0.866605},
		{ -1.43722,   0.904334,   0.617428,   1.17754},
		{ -1.47288,   0.316685,  -0.962764, -0.394483},
		{-0.374311,   0.313911,   -1.27085, -0.203249},
		{-0.166439,   -1.56434,   0.858862,  -1.05044}
	};

	elementNode* chest;
	elementNode* head;
	elementNode* muzzle;
	elementNode* earLeft;
	elementNode* earRight;
	elementNode* legFrontLeft;
	elementNode* legFrontRight;
	elementNode* legBackLeft;
	elementNode* legBackRight;
	elementNode* tail;

	hingeConstraint* hinge_body_head;
	hingeConstraint* hinge_head_muzzle;
	hingeConstraint* hinge_earLeft_head;
	hingeConstraint* hinge_earRight_head;
	hingeConstraint* hinge_body_legFrontLeft;
	hingeConstraint* hinge_body_legFrontRight;
	hingeConstraint* hinge_body_legBackLeft;
	hingeConstraint* hinge_body_legBackRight;
	hingeConstraint* hinge_body_tail;

	phaseOscillator *osci = new phaseOscillator(4, POdegreeOfFourier);

	static constexpr double phi_max = M_PI/3;
	double phi[4];


	public:

	dog (float x, float y, float z) {
		spawn(x, y, z);
	}


	void spawn(float x, float y, float z){

		for(double p: phi){
			p = 0.0;
		}
		this->osci->resetOscillator();

		//犬の体の構造を定義している
		//キューブで肉体を作る cubeshape::create(位置, 大きさ, 傾き, 重さ, 追加先物理世界);

		chest = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(    x,     y,     z)),
					param("scale",    Eigen::Vector3f(  1, 0.5, 0.5)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 20.0f)));

		head = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4,     y,     z)),
					param("scale", Eigen::Vector3f(0.4, 0.4, 0.4)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 2.0f)));

		muzzle = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+2.1, y-0.2,     z)),
					param("scale", Eigen::Vector3f(0.3, 0.2, 0.2)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.5f)));

		earLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4, y+0.5, z-0.2)),
					param("scale", Eigen::Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.1f)));

		earRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4, y+0.5, z+0.2)),
					param("scale", Eigen::Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.1f)));

		legFrontLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+0.5,   y-1, z-0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 3.0f)));

		legFrontRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+0.5,   y-1, z+0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 3.0f)));

		legBackLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x-0.5,   y-1, z-0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 3.0f)));

		legBackRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x-0.5,   y-1, z+0.4)),
					param("scale", Eigen::Vector3f(0.1, 0.5, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 3.0f)));

		tail = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x-1.5, y+0.4,     z)),
					param("scale", Eigen::Vector3f(0.5, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 2.0f)));


		hinge_body_head			= new hingeConstraint(chest   , head         , Eigen::Vector3f(   1,    0,    0), Eigen::Vector3f(-0.4,   0,    0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_head_muzzle		= new hingeConstraint(head    , muzzle       , Eigen::Vector3f( 0.4, -0.2,    0), Eigen::Vector3f(-0.3,   0,    0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_earLeft_head		= new hingeConstraint(earLeft , head         , Eigen::Vector3f(   0, -0.1,    0), Eigen::Vector3f(   0, 0.4, -0.2), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_earRight_head		= new hingeConstraint(earRight, head         , Eigen::Vector3f(   0, -0.1,    0), Eigen::Vector3f(   0, 0.4,  0.2), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legFrontLeft = new hingeConstraint(chest   , legFrontLeft , Eigen::Vector3f( 0.5, -0.5, -0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legFrontRight= new hingeConstraint(chest   , legFrontRight, Eigen::Vector3f( 0.5, -0.5,  0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legBackLeft	= new hingeConstraint(chest   , legBackLeft  , Eigen::Vector3f(-0.5, -0.5, -0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_legBackRight	= new hingeConstraint(chest   , legBackRight , Eigen::Vector3f(-0.5, -0.5,  0.4), Eigen::Vector3f(   0, 0.5,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));
		hinge_body_tail			= new hingeConstraint(chest   , tail         , Eigen::Vector3f(  -1,  0.4,    0), Eigen::Vector3f( 0.5,   0,  0.0), Eigen::Vector3f(0, 0, 1), Eigen::Vector3f(0, 0, 1));

		hinge_body_head->setLimit(-M_PI/9, M_PI/9);
		hinge_head_muzzle->setLimit(0, 0);
		hinge_earLeft_head->setLimit(0, 0);
		hinge_earRight_head->setLimit(0, 0);
		hinge_body_legFrontLeft->setLimit(-phi_max, phi_max);
		hinge_body_legFrontRight->setLimit(-phi_max, phi_max);
		hinge_body_legBackLeft->setLimit(-phi_max, phi_max);
		hinge_body_legBackRight->setLimit(-phi_max, phi_max);
		hinge_body_tail->setLimit(-M_PI/3, M_PI/3);

		hinge_body_legFrontLeft->enableMotor(true);
		hinge_body_legFrontLeft->setMaxMotorImpulse(2);
		hinge_body_legFrontRight->enableMotor(true);
		hinge_body_legFrontRight->setMaxMotorImpulse(2);
		hinge_body_legBackLeft->enableMotor(true);
		hinge_body_legBackLeft->setMaxMotorImpulse(2);
		hinge_body_legBackRight->enableMotor(true);
		hinge_body_legBackRight->setMaxMotorImpulse(2);

		this->initPosition = Eigen::Vector3f(
										this->head->getXpos(), 
										this->head->getYpos(),
										this->head->getZpos()
											);

	}

	Eigen::Vector3f getPosition(){
		return Eigen::Vector3f(
						this->head->getXpos(), 
						this->head->getYpos(),
						this->head->getZpos()
							);
	}


	void phi2theta(){
		for(int l=0; l<4; l++){

			//物理演算中にphiがphi_maxを超えた場合は+-phi_maxとして処理
			//thetaは0かpiとして返す
			if(this->phi[l] >= this->phi_max){
				this->phi[l] = this->phi_max;
				this->osci->theta[l] = 0.0;
				return;
			}else if(this->phi[l]<= -1.0*this->phi_max){
				this->phi[l] = -1.0*this->phi_max;
				this->osci->theta[l] = M_PI;
				return;
			}

			//thetaが1, 2象限なのか3, 4象限なのかで場合分け
			if(this->osci->theta[l]<=M_PI){
				this->osci->theta[l] = acos( sin(this->phi[l]) / sin(this->phi_max) );
			}else{
				this->osci->theta[l] = -1.0*acos( sin(this->phi[l]) / sin(this->phi_max) ) + 2.0*M_PI;
			}

		}
	}

	void theta2phi(){
		for(int l=0; l<4; l++){
			this->phi[l] = asin( sin(this->phi_max) * cos(this->osci->theta[l]) );
		}
	}


	void move(int sequence){

		if(phaseOsci){
			//phaseOscillator
			phi[0] = hinge_body_legFrontLeft->getHingeAngle();
			phi[1] = hinge_body_legFrontRight->getHingeAngle();
			phi[2] = hinge_body_legBackLeft->getHingeAngle();
			phi[3] = hinge_body_legBackRight->getHingeAngle();

			this->phi2theta();
			this->osci->calTheta();
			this->theta2phi();

			hinge_body_legFrontLeft->setMotorTarget(this->phi[0], 0.1);
			hinge_body_legFrontRight->setMotorTarget(this->phi[1], 0.1);
			hinge_body_legBackLeft->setMotorTarget(this->phi[2], 0.1);
			hinge_body_legBackRight->setMotorTarget(this->phi[3], 0.1);
		}else{

			//serialOrder
			hinge_body_legFrontLeft->setMotorTarget(dna[sequence][0], 0.3);
			hinge_body_legFrontRight->setMotorTarget(dna[sequence][1], 0.3);
			hinge_body_legBackLeft->setMotorTarget(dna[sequence][2], 0.3);
			hinge_body_legBackRight->setMotorTarget(dna[sequence][3], 0.3);

		}

	}


	void despawn(){
		hinge_body_head->destroy();
		hinge_head_muzzle->destroy();
		hinge_earLeft_head->destroy();
		hinge_earRight_head->destroy();
		hinge_body_legFrontLeft->destroy();
		hinge_body_legFrontRight->destroy();
		hinge_body_legBackLeft->destroy();
		hinge_body_legBackRight->destroy();
		hinge_body_tail->destroy();


		chest->destroy();
		head->destroy();
		muzzle->destroy();
		earLeft->destroy();
		earRight->destroy();
		legFrontLeft->destroy();
		legFrontRight->destroy();
		legBackLeft->destroy();
		legBackRight->destroy();
		tail->destroy();
	}
};


//--------------------------------------------------------------------
//init and simulating loop
//--------------------------------------------------------------------

std::vector<dog*> doglist;
dog* meanDog;

//for CMA-ES
using precision = double;
using vec = Eigen::Matrix<precision, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<precision, Eigen::Dynamic, Eigen::Dynamic>;
const int N = phaseOsci? 4+2*POdegreeOfFourier*4*(4-1):80;
const int maxiter = 700;

std::function<precision(vec)> func = sphere;
cmaes es(
		func,
		vec::Zero(N), 1.0, numofdog
		);

//initialize val for record
vec topf = vec::Zero(maxiter);
vec meanf = vec::Zero(maxiter);
vec sigma = vec(maxiter);
mat D = mat::Zero(maxiter, N);
mat diagC = mat::Zero(maxiter, N);

std::chrono::system_clock::time_point start, end;

//for taking mean of multiple trials in one itr.
const int evaluateMeanOf = 5;
int meanCount = 0;
double meanReaching = 0.0;
double reachingDistance[numofdog] = {0.0};

int esItr = 0;
int lastitr = 0;
int timerDivisor = 0;
int clockOfTrial = 0;
const int limitOfTrial = 500;
int sequence = 0;
float topOfTrial = -1000000000;
vec topDogCoeff;

extern "C"
void init() {
	start = std::chrono::system_clock::now();

	std::cout<<"maxiter : "<<maxiter<<std::endl;
	std::cout<<"trial time : "<<limitOfTrial<<std::endl;
	std::cout<<"controller : ";
	if(phaseOsci){
		std::cout<<"phase oscillator"<<std::endl;
	}else{
		std::cout<<"serial order"<<std::endl;
	}
	std::cout<<"take a mean of "<<evaluateMeanOf<<" trials in one itr"<<std::endl;


	es.generateSample();

	meanDog = new dog(0, 1.5, 10.0);
	if(phaseOsci){

		for(int i=0; i<N; i++){
			meanDog->osci->coeff[i] = es.mean(i);
		}
		for(int n = 0; n < numofdog; n++){

			doglist.push_back(new dog(0, 1.5, -10.0*n));
			for(int i=0; i<20; i++){
				doglist[n]->osci->coeff[i] = es.sample.row(n)(i);
			}
		}

	}else{

		int c=0;
		for(int i=0; i<20; i++){
			for(int j=0;j<4;j++){
				meanDog->dna[i][j] = es.mean(c++);
			}
		}
		for (int n = 0; n < numofdog; n++) {
			doglist.push_back(new dog(0, 1.5, -10.0*n));
			doglist[n]->initPosition = doglist[n]->getPosition();
			c = 0;
			for(int i=0; i<20; i++){
				for(int j=0;j<4;j++){
					doglist[n]->dna[i][j] = es.sample.row(n)(c++);
				}
			}
		}

	}

	std::cout<<"itr 0 starts.-------------"<<std::endl;

}


extern "C"
void tick() {

	//std::cout<<"\t\tclock : "<<clockOfTrial<<std::endl;

	//dogs move
	if(timerDivisor++ == 5){
		sequence = (sequence+1)%20;
		timerDivisor = 0;
		meanDog->move(sequence);
		for (auto elem : doglist){
			elem->move(sequence);
		}
	}

	//end a trial
	if(++clockOfTrial==limitOfTrial){
		lastitr = esItr;

		std::cout<<"\t"<<(meanCount+1)<<"/"<<evaluateMeanOf<<" ends."<<std::endl;

		//evaluation
		double meanReach = 0.0;
		meanReach = meanDog->getPosition()[0] - meanDog->initPosition[0];
		meanReaching += meanReach;
		double topOfThisCount = -100000000000.0;
		for(int n=0; n<numofdog; n++){
			double reach = 0.0;
			reach = doglist[n]->getPosition()[0] - doglist[n]->initPosition[0];
			reachingDistance[n] += reach;
			if(topOfThisCount<reach){
				topOfThisCount = reach;
			}
		}
		std::cout<<"\t\ttop : "<<topOfThisCount<<std::endl;
		std::cout<<"\t\tmeanDog : "<<meanReach<<std::endl;

		//goodbye dogs
		meanDog->despawn();
		for(int n=0; n<numofdog; n++){
			doglist[n]->despawn();
		}

		//hello dogs
		meanDog->spawn(0, 1.5, 10.0);
		for(int n = 0; n < numofdog; n++){
			doglist[n]->spawn(0, 1.5, -10.0*n);
		}

		//itr ends.
		if(++meanCount == evaluateMeanOf){


			//record

			for(int n=0; n<numofdog; n++){
				if(topOfTrial<reachingDistance[n]/double(evaluateMeanOf)){
					topOfTrial = reachingDistance[n]/double(evaluateMeanOf);
					topDogCoeff = es.sample.row(n);
				}
				es.arf(n) = -1.0*reachingDistance[n];
				reachingDistance[n] = 0.0;
			}

			topf(esItr) = -1.0*topOfTrial;
			meanf(esItr) = -1.0*meanReaching/double(evaluateMeanOf);
			sigma(esItr) = es.sigma;
			D.row(esItr) = es.D.transpose();
			diagC.row(esItr) = es.C.diagonal().transpose();

			std::cout<<"end."<<std::endl;
			std::cout<<"\ttop : "<<topOfTrial<<std::endl;
			std::cout<<"\tmeanDog : "<<meanReaching/double(evaluateMeanOf)<<std::endl;
			meanReaching = 0.0;

			topOfTrial = -10000000;

			//one es generation ends
			es.updateParam();
			es.generateSample();

			//goodbye dogs
			meanDog->despawn();
			for(int n=0; n<numofdog; n++){
				doglist[n]->despawn();
			}

			//hello dogs
			if(phaseOsci){

				meanDog->spawn(0, 1.5, 10.0);
				for(int i=0; i<N; i++){
					meanDog->osci->coeff[i] = es.mean(i);
				}
				for(int n = 0; n < numofdog; n++){
					doglist[n]->spawn(0, 1.5, -10.0*n);
					for(int i=0; i<20; i++){
						doglist[n]->osci->coeff[i] = es.sample.row(n)(i);
					}
				}

			}else{

				meanDog->spawn(0, 1.5, 10.0);
				int c=0;
				for(int i=0; i<20; i++){
					for(int j=0;j<4;j++){
						meanDog->dna[i][j] = es.mean(c++);
					}
				}
				for (int n = 0; n < numofdog; n++) {
					doglist[n]->spawn(0, 1.5, -10.0*n);
					c = 0;
					for(int i=0; i<20; i++){
						for(int j=0;j<4;j++){
							doglist[n]->dna[i][j] = es.sample.row(n)(c++);
						}
					}
				}

			}

			meanCount = 0;
			std::cout<<"itr "<<esItr<<" starts.-------------"<<std::endl;
			esItr++;
		}
		clockOfTrial = 0;

	}

	//end learning
	if(esItr==maxiter){
		//std::cout<<meanf<<std::endl;

		export_data("plugins/result/es_result_topParam.csv", topDogCoeff);
		export_data("plugins/result/es_result_topf.csv", topf);
		export_data("plugins/result/es_result_meanf.csv", meanf);
		export_data("plugins/result/es_result_sigma.csv", sigma);
		export_data("plugins/result/es_result_D.csv", D);
		export_data("plugins/result/es_result_diagC.csv", diagC);

		end = std::chrono::system_clock::now();
		double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
		std::cout<<"total time : "<<elapsed<<"msec"<<std::endl;

		exit(0);
	}



}
