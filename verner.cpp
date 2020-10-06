// Semyon Malykh
// malykhsm@gmail.com
// Nizhniy Novgorod 2019

#pragma once 

#include <iostream>
#include <memory> 

#include "types.h"
#include "consts.h"
#include "dverk.h"

void dverkStep(double* val, const int32_t dimension, EquationFunc* rightSideFunc, double step) {
	int32_t memsize = sizeof(double) * dimension;
	double* arg = (double*)malloc(memsize);
	double* k1 = (double*)malloc(memsize);
	double* k2 = (double*)malloc(memsize);
	double* k3 = (double*)malloc(memsize);
	double* k4 = (double*)malloc(memsize);
	double* k5 = (double*)malloc(memsize);
	double* k6 = (double*)malloc(memsize);
	double* k7 = (double*)malloc(memsize);
	double* k8 = (double*)malloc(memsize);
	
	rightSideFunc->func(val, k1, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step / 6 * k1[j];
	}
	rightSideFunc->func(arg, k2, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step * (4. / 75 * k1[j] + 16. / 75 * k2[j]);
	}
	rightSideFunc->func(arg, k3, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step * (5. / 6 * k1[j] - 8. / 3 * k2[j] + 5. / 2 * k3[j]);
	}
	rightSideFunc->func(arg, k4, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step * (-165. / 64 * k1[j] + 55. / 6 * k2[j] - 425. / 64 * k3[j] + 85. / 96 * k4[j]);
	}
	rightSideFunc->func(arg, k5, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step * (12. / 5 * k1[j] - 8. * k2[j] + 4015. / 612 * k3[j] - 11. / 36 * k4[j] + 88. / 255 * k5[j]);
	}
	rightSideFunc->func(arg, k6, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step * (-8263. / 15000 * k1[j] - 124. / 75 * k2[j] - 643. / 680 * k3[j] - 81. / 250 * k4[j] + 2484. / 10625 * k5[j]);
	}
	rightSideFunc->func(arg, k7, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		arg[j] = val[j] + step * (3501. / 1720 * k1[j] - 300. / 43 * k2[j] + 297275. / 52632 * k3[j] - 319. / 2322 * k4[j] + 24068. / 84065 * k5[j] + 3850. / 26703 * k7[j]);
	}
	rightSideFunc->func(arg, k8, rightSideFunc->params);
	for (int32_t j = 0; j < dimension; j++) {
		val[j] = val[j] + step * (3. / 40 * k1[j] + 875. / 2244 * k3[j] + 23. / 72 * k4[j] + 264. / 1955 * k5[j] + 125. / 11592 * k7[j] + 43. / 616 * k8[j]);
	}
	free(arg);
	free(k1);
	free(k2);
	free(k3);
	free(k4);
	free(k5);
	free(k6);
	free(k7);
	free(k8);
}
