#ifndef __MATH_H__
#define __MATH_H__

#define MATH_D_LIMIT(MAX, MIN, DATA) ((DATA > MAX) ? MAX : ((DATA < MIN) ? MIN : DATA))
#define MATH_D_ABS(DATA) (DATA > 0) ? DATA : (-DATA)

#define MATH_D_RELATIVE_CAL_PARAM 0.000767944870878f

float MATH_F_Sqrt(float DATA);

#endif // !__MATH_H__