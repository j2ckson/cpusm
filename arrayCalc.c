#include "./arrayCalc.h"

double max_calc(const double stat, const double val)
{
	double max = stat > val ? stat : val;
	return max;
}
double min_calc(const double stat, const double val)
{
	double min = ( stat < val && stat > 0 ) ? stat : val > 0 ? val : stat;
	return min;
}
double mean_calc(const double stat, const double val, const int count)
{
	double mean = stat*(count-1)/count + val*1/count;
	return mean;
}
void arraySet(const double array0[], double array1[], const int arraySize, const int arrayOffset)
{
	for ( int i=0;i<arraySize;i++ ) {
		array1[i] = array0[i+arrayOffset];
	}
	return;
}
void arrayX(const double array0[], double arrayMin[], double arrayMax[], double arrayMean[], const int arraySize, const int count)
{
	for ( int i=0;i<arraySize;i++ ) {
		arrayMax[i] = max_calc(array0[i], arrayMax[i]);
		arrayMin[i] = min_calc(array0[i], arrayMin[i]);
		arrayMean[i] = mean_calc(arrayMean[i], array0[i], count);
	}
	return;
}
void arrayStat(const double array0[], const double array1[], double array2[], const int arraySize)
{
	for ( int i=0;i<arraySize;i++ ) {
		array2[i] = array0[i] + array1[i];
	}
	return;
}
double arraySum(const double array0[], const int arraySize)
{
	double arraySumation = 0L;
	for ( int i=0;i<arraySize;i++ ) {
		arraySumation += array0[i];
	}
	return arraySumation;
}
double arrayAvg(const double array0[], const int arraySize)
{
	double arrayTotal = arraySum(array0, arraySize);
	double arrayAverage = arrayTotal/arraySize;
	return arrayAverage;
}
double arraySub(const double array0[], const double array1[], double array2[], const int arraySize)
{
	double arraySubtraction = 0L;
	for ( int i=0;i<arraySize;i++ ) {
		array2[i] = array0[i] - array1[i];
		arraySubtraction += array2[i];
	}
	return arraySubtraction;
}
void arrayInit(double array0[], const int arraySize)
{
	for ( int i=0;i<arraySize;i++ ) {
		array0[i] = 0;
	}
	return;
}
void arrayDiv(double array0[], const double divisor, const int arraySize)
{
	for ( int i=0;i<arraySize;i++ ) {
		array0[i] /= divisor;
	}
	return;
}
