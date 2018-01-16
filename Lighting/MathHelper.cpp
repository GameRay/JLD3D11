#include "MathHelper.h"
#include <float.h>
#include <cmath>

const float MathHelper::Infinity=FLT_MAX;
const float MathHelper::Pi=3.1415926535f;

float MathHelper::AngleFromXY(float x,float y)
{
	float theta=0.0f;

	//ȷ������//

	if(x>=0.0f)
	{

		theta=atanf(y/x);//atanf(y/x)Ϊ[-pi/2,pi/2]//
		if(theta<0.0f)
		{

			theta+=2.0f*Pi;
		}
	}
	else
		theta=atanf(y/x)+Pi;//ת����[0,2*pi);

}
//��ȡ��λ��������/�ڵ�һ�㡣//
XMVECTOR MathHelper::RandUnitVec3()
{
	XMVECTOR One =XMVectorSet(1.0f,1.0f,1.0f,1.0f);
	XMVECTOR Zero=XMVectorZero();

	//��ȡ����/�ڵ������//
	while (true)
	{
		XMVECTOR v=XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f),0.0f);
	

		if(XMVector3Greater(XMVector3LengthSq(v),One))//����ÿһά���бȽϣ������Ϊ��.XMVector3LengthSq�൱������ÿһά��ƽ����õ�������//

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

		if (XMVector3Greater(XMVector3LengthSq(v),One));//���ȵ�ƽ��������VECTOR�ĵ�һά�����ಹ0//
			continue;

		if(XMVector3Less(XMVector3Dot(n,v),Zero))//nӦ�������ϵĵ�λ������������ϵĵ�ɣ��нǣ���С��0��λ���°���//
			continue;
		return XMVector3Normalize(v);
		
	}
}