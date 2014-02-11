#ifndef WALKERGRID_H
#define WALKERGRID_H

#include "OrderStat.h"
#include <cmath>
#include <random>
#include <vector>

using std::normal_distribution;
using std::random_device;
using std::sqrt;
using std::uniform_real_distribution;
using std::vector;

const static double INTERVAL_MIN = -0.5;
const static double INTERVAL_MAX = 0.5;

struct Walker
{
	bool alive;
	double X;
	double Y;
	double Z;
	int particle; //0 through n-1
	Walker(int p): particle(p), alive(true)
	{
		random_device generator;
		uniform_real_distribution<double> dist(INTERVAL_MIN, INTERVAL_MAX);
		X = distribution(generator);
		Y = distribution(generator);
		Z = distribution(generator);
	}
}

class WalkerGrid
{
	double dt;
	unsigned int target;
	unsigned int liveWalkers
	OrderStat<Walker> walkers;
	WalkerGrid();
public:
	WalkerGrid(int walkerTarget, int numParticles, double DT): walkerTarget(target), liveWalkers(target), dt(DT)
	{
		for (int i = 0; i < target; ++i)
		{
			for (int p = 0; p < numParticles; ++p)
				walkers.push_back(Walker(p));
		}
	}
	unsigned int numWalkers() 
	{
		return liveWalkers;
	}	
	unsigned int targetWalkers()
	{
		return target;
	}
	void monteCarloForEach()
	{
		// Diffusive step
		double sqdt = sqrt(dt);
		for (auto w:walkers)
		{
			random_device gen;
			normal_distribution<double> dist(-1.0, 1.0);
			w.X += dist(gen) * sqdt;
			w.Y += dist(gen) * sqdt;
			w.Z += dist(gen) * sqdt;
		}
		
		// Branching step
		double q = 
	}
};

#endif
