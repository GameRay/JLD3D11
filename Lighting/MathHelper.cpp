#include "MathHelper.h"
#include <float.h>
#include <cmath>

const float MathHelper::Infinity=FLT_MAX;
const float MathHelper::Pi=3.1415926535f;

float MathHelper::AngleFromXY(float x,float y)
{
	float theta=0.0f;

	//确定象限//

	if(x>=0.0f)
	{

		theta=atanf(y/x);//atanf(y/x)为[-pi/2,pi/2]//
		if(theta<0.0f)
		{

			theta+=2.0f*Pi;
		}
	}
	else
		theta=atanf(y/x)+Pi;//转换到[0,2*pi);

}
//获取单位向量球上/内的一点。//
XMVECTOR MathHelper::RandUnitVec3()
{
	XMVECTOR One =XMVectorSet(1.0f,1.0f,1.0f,1.0f);
	XMVECTOR Zero=XMVectorZero();

	//获取球上/内的随机点//
	while (true)
	{
		XMVECTOR v=XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f),0.0f);
	

		if(XMVector3Greater(XMVector3LengthSq(v),One))//向量每一维进行比较，都大的为大.XMVector3LengthSq相当于向量每一维的平方后得到的响亮//

			continue;
		return XMVector3Normalize(v);
	}
}
XMVECTOR MathHelper::RandHemisphereUnitVec3(XMVECTOR n)
{
	XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	while(true)
	{
		XMVECTOR v=XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		if (XMVector3Greater(XMVector3LengthSq(v),One));//长度的平方，存入VECTOR的第一维，其余补0//
			continue;

		if(XMVector3Less(XMVector3Dot(n,v),Zero))//n应该是向上的单位向量，球和向上的点成（夹角），小于0则位于下半球//
			continue;
		return XMVector3Normalize(v);
		
	}
}