#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <limits>
#include "cmaes/cmaes.cpp"
#include "cmaes/objective_func.cpp"
#include "cmaes/func_es.cpp"
#include "cmaes/export.cpp"
#include "phaseOscillator.cpp"
#include "../../source/japarilib.hpp"

template <typename... ARGS>
parameterPack* paramPack(ARGS... args){
	return new parameterPack(sizeof... (ARGS), args...);
}



const int numofdog = 100;
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

	phaseOscillator *osci = new phaseOscillator(4, 3);

	double phi_max = M_PI/4;
	double phi[4];


	public:

	dog (float x, float y, float z) {
		spawn(x, y, z);
	}


	void spawn(float x, float y, float z){

		for(double p: phi){
			p = 0.0;
		}

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
					param("mass", 2.5f)));

		muzzle = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+2.1, y-0.2,     z)),
					param("scale", Eigen::Vector3f(0.3, 0.2, 0.2)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 1.0f)));

		earLeft = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4, y+0.5, z-0.2)),
					param("scale", Eigen::Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.5f)));

		earRight = getCubeshape()->generate(paramPack(
					param("position", Eigen::Vector3f(x+1.4, y+0.5, z+0.2)),
					param("scale", Eigen::Vector3f(0.1, 0.1, 0.1)),
					param("rotation", Eigen::Quaternionf(1, 0, 0, 0)),
					param("mass", 0.5f)));

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

		hinge_body_head->setLimit(-M_PI/6, M_PI/6);
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

		/*
		hinge_body_legFrontLeft->setMotorTarget(dna[sequence][0], 0.5);
		hinge_body_legFrontRight->setMotorTarget(dna[sequence][1], 0.5);
		hinge_body_legBackLeft->setMotorTarget(dna[sequence][2], 0.5);
		hinge_body_legBackRight->setMotorTarget(dna[sequence][3], 0.5);
		*/
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


std::vector<dog*> doglist;

//for CMA-ES
using precision = float;
using vec = Eigen::Matrix<precision, Eigen::Dynamic, 1>;
using mat = Eigen::Matrix<precision, Eigen::Dynamic, Eigen::Dynamic>;

int esItr = 0;
int lastitr = 0;
const int maxiter = 50;
const int N = 4 + 2*3*4*3; //numOfOsci + 2*degreeOfFourier*numOfOsci*(numOfOsci-1)

std::function<precision(vec)> func = sphere<precision>;
cmaes<precision> es(
		func,
		0.3*vec::Ones(N), 0.1, numofdog
		);

//initialize val for record
vec meanf = vec::Zero(maxiter);
vec sigmaN = vec(maxiter);
mat D = mat::Zero(maxiter, N);
mat diagC = mat::Zero(maxiter, N);

extern "C"
void init() {

	std::cout<<"maxiter : "<<maxiter<<std::endl;

	es.generateSample();
	for (int i = 0; i < numofdog; i++) {
		doglist.push_back(new dog(0, 1.5, -5*i));
		doglist[i]->initPosition = doglist[i]->getPosition();
		doglist[i]->osci->coeff = es.sample.row(i);
	}

}

int timerDivisor = 0;
int clockOfTrial = 0;
const int limitOfTrial = 500;
int sequence = 0;
float topOfTrial = -1000000000;


extern "C"
void tick() {

	//start an itr
	if(clockOfTrial==limitOfTrial){
		std::cout<<"itr "<<esItr<<" ends."<<std::endl;
		lastitr = esItr;

		//evaluation
		for(int n=0; n<numofdog; n++){
			float reachingDistance = (doglist[n]->getPosition()[0] - doglist[n]->initPosition[0]);
			es.arf(n) = -1.0*reachingDistance; //esは最小値を探す
			topOfTrial = std::max(topOfTrial, reachingDistance);
		}
		std::cout<<"top : "<<topOfTrial<<std::endl;
		topOfTrial = -10000000;

		//goodbye dogs
		for(int n=0; n<numofdog; n++){
			doglist[n]->despawn();
		}
		es.generateSample();
		//hello dogs
		for (int n = 0; n < numofdog; n++) {
			doglist[n]->spawn(0, 1.5, -5*n);
			doglist[n]->osci->coeff = es.sample.row(n);
		}
	}

	//dogs move
	if(timerDivisor++ == 6){
		sequence = (sequence+1)%20;
		timerDivisor = 0;
		for (auto elem : doglist){
			elem->move(sequence);
		}
	}

	//end one itr
	if(clockOfTrial==limitOfTrial){

		es.updateParam();

		//record
		meanf(esItr) = es.func(es.mean);
		sigmaN(esItr) = es.sigma*es.N;
		D.row(esItr) = es.D.transpose();
		diagC.row(esItr) = es.C.diagonal().transpose();

		if(isnan(meanf(esItr))){
			std::cout<<"func(mean) goes nan"<<std::endl;
			exit(0);
		}
		if((esItr!=0)&&(meanf(esItr)==0)){
			std::cout<<"func(mean) goes 0(at itr="<<lastitr<<")"<<std::endl;
			exit(0);
		}

		esItr++;
		clockOfTrial = 0;
	}

	if(esItr==maxiter){
		//std::cout<<meanf<<std::endl;
		meanf = meanf.block(0, 0, lastitr, 1);

		export_data<precision>("result/es_result_meanf.csv", meanf);
		export_data<precision>("result/es_result_sigmaN.csv", sigmaN);
		export_data<precision>("result/es_result_D.csv", D);
		export_data<precision>("result/es_result_diagC.csv", diagC);

		exit(0);
	}

	clockOfTrial++;

}
