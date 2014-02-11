#include <cmath>
#include <cstdio>
#include <iostream>
#include <random>
#include <vector>
#include "WalkerGrid.h"

using std::cin;
using std::cout;
using std::endl;
using std::fclose;
using std::fopen;
using std::fread;
using std::fwrite;
using std::log;
using std::vector;

double oneTimeStep (const WalkerGrid &w, double E_T) //returns energy for that timestep (I think)
{
	w.monteCarloForEach();
	w.removeAllDeadWalkers();
	E_T += log(w.targetWalkers() / static_cast<double> w.numWalkers()) / 10;
	//////////////////////////
	//			//
	//	UPDATE PSI	//
	//			//
	//////////////////////////
	return E_T;	
}

int main(int argc, char *argv[])
{
	//what are the units for time here? seconds? ns?
	unsigned int timeSteps = 2000;		//to be specified by user
	unsigned double dt = 0.01;			//to be specified by user	
	unsigned int numWalkers = 2000; 	//to be specified by user
	unsigned int numParticles = 2;		//to be specified by user
	unsigned int spacialDims = 3;		
	unsigned double r0 = sqrt(0.1);
	unsigned int DIM = spacialDims*numParticles;
	
	//define observables
	double netEnergy = 0.0;
	double rMax = 10;
	const int psiBins = 100;
	vector<double> psi(psiBins);	

	for (double V0 = 0.0; V0 >= -100; ++V0)
	{
		//initialize
		double targetEnergy = 0.0;	
		WalkerGrid grid(numWalkers, numParticles);
	
		int thermSteps = static_cast<int>(0.2*timeSteps); 	//what's the difference between a thermalization step and a time step?
									// NYI maybe??
		for (int i = 0; i < thermSteps; ++i)
		{
			oneTimeStep(grid);
		}
		zeroAccumulators();
		for (int i = 0; i < timeSteps; ++i)
		{
			oneTimeStep();
		}
		//double energyAvg = energyNet / timeSteps;
		//double energyVar = e2Net / timeSteps - energyAvg * energyAvg;
	
	}
	//////////////////////////////////////////////////////////
	//							//
	//							//
	//							//
	// write all this to a file for interpretation by   	//
	//            R/pylab/mathematica			//
	//////////////////////////////////////////////////////////
	
	return 0;
}
