// Semyon Malykh
// malykhsm@gmail.com
// Nizhniy Novgorod 2019

#pragma once 

#include <iostream>
#include <memory> 

#include "types.h"
#include "consts.h"
#include "shimizu.h"
#include "rungeKutt.h"

 double* rungeKutt(const double *initVals, const int initValsSize, EquationFunc* rightSideFunc, double step, int32_t integrationTime) {
	 int32_t numIterations;
	 double lastStep = 0.;

	 if (((integrationTime / step) - (int32_t)(integrationTime / step)) != 0.0) {	  // If integrationTime do not devided entirely by step
		 numIterations = (int32_t)(integrationTime / step) + 1;						 // Additional step to ritch the integration time
		 lastStep = (double)integrationTime - ((double)(numIterations - 1) * step);	// The last step size
	 }
	 else
		 numIterations = (int32_t)(integrationTime / step);
 
	int32_t memsize = sizeof(double) * initValsSize;
	double* res = (double*)malloc(memsize * numIterations);
	double* val = (double*)malloc(memsize);
	double* arg = (double*)malloc(memsize);
	double* k1 = (double*)malloc(memsize);
	double* k2 = (double*)malloc(memsize);
	double* k3 = (double*)malloc(memsize);
	double* k4 = (double*)malloc(memsize);
	memcpy(val, initVals, sizeof(double) * initValsSize);
	for (int32_t i = 0; i < numIterations; i++){
		if (lastStep != 0.)					//If optimization needed remove from loop
			if (i == numIterations - 1)
				step = lastStep;
		rightSideFunc->func(val, k1, rightSideFunc->params);
		for (int32_t j = 0; j < initValsSize; j++){
			arg[j] = val[j] + step / 2 * k1[j];
		}
		rightSideFunc->func(arg, k2, rightSideFunc->params);
		for (int32_t j = 0; j < initValsSize; j++){
			arg[j] = val[j] + step / 2 * k2[j];
		}
		rightSideFunc->func(arg, k3, rightSideFunc->params);
		for (int32_t j = 0; j < initValsSize; j++){
			arg[j] = val[j] + step * k3[j];
		}
		rightSideFunc->func(arg, k4, rightSideFunc->params);
		for (int32_t j = 0; j < initValsSize; j++){
			val[j] = val[j] + (k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j]) * ( step / 6 );
		}
		for (int32_t j = 0; j < initValsSize; j++){
			res[i * initValsSize + j] = val[j];
		}
	}
	free(val);
	free(arg);
	free(k1);
	free(k2);
	free(k3);
	free(k4);
	return res;
}
