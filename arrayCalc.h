#ifndef ARRAYCALC_H
#define ARRAYCALC_H

double max_calc(const double stat, const double val);
double min_calc(const double stat, const double val);
double mean_calc(const double stat, const double val, const int count);
void arraySet(const double array0[], double array1[], const int arraySize, const int arrayOffset);
void arrayX(const double array0[], double arrayMin[], double arrayMax[], double arrayMean[], const int arraySize, const int count);
void arrayStat(const double array0[], const double array1[], double array2[], const int arraySize);
double arraySum(const double array0[], const int arraySize);
double arrayAvg(const double array0[], const int arraySize);
double arraySub(const double array0[], const double array1[], double array2[], const int arraySize);
void arrayInit(double array0[], const int arraySize);
void arrayDiv(double array0[], const double divisor, const int arraySize);

#endif
