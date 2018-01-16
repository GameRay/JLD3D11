#ifndef MATHHELPER_H
#define MATHHELPER_H
#include<Windows.h>
#include <xnamath.h>

class MathHelper
{
public:


	static float RandF()
	{
		return (float)(rand())/(float)RAND_MAX;//rand()����0-RAND_MAX�������
	}



	static float RandF(float a,float b)
	{

		return a+RandF()*(b-a);
	}
};
#endif
