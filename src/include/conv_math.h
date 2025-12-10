#ifndef CONV_MATH_H
#define CONV_MATH_H

#include "funcs.h"

int KfromV(converter *active);
int KfromI(converter *active);
int VofromOhm(converter *active);
int VofromK(converter *active);
int VifromK(converter *active);
int IofromOhm(converter *active);
int IofromK(converter *active);
int IifromK(converter *active);

int calc_L(converter *active);
int calc_Co(converter *active);
int calc_Cn(converter *active);
int calc_L2(converter *active);

#endif /*CONV_MATH_H*/