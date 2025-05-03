#ifndef __MATH_H__
#define __MATH_H__

#define MATH_D_LIMIT(MAX, MIN, DATA) ((DATA > MAX) ? MAX : ((DATA < MIN) ? MIN : DATA))
#define MATH_D_ABS(DATA) ((DATA) > 0) ? (DATA) : (-DATA)

// #define MATH_D_RELATIVE_PARAM 0.000767944870878f
#include "main.h" 
#define MATH_D_RELATIVE_PARAM 0.000766990393942820556640625f
float MATH_F_Sqrt(float DATA);
float LimitRange(float value, float MAX_VALUE, float MIN_VALUE);
void convertAngleToIndex(float angle, float *index);
float convertInsexToAngle(float value);
float spinLittleRound(double *input);
// float spinLittleRound_l(double *input);
float mouseFilter(float last, float now, float thresholdValue);
int abs_int(int16_t data);
float SectionLimit_f(float max , float min , float data);

float YU_MATH_LowPassFilter(float alpha, float *input);
float YU_MATH_LowPassFilter_MC(float alpha, float *input, uint8_t channel);


#endif // !__MATH_H__