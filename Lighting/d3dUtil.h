#ifndef  D3DUTIL_H
#define  D3DUTIL_H

#if defined(DEBUG)||defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>////1.ʹ��_PRTn�ĺ���ʾ������Ϣ 2.����ڴ�й©��_CrtSetDbgFlag�������������������
#endif

#include <D3DX11.h>
#include "d3dx11effect.h"
#include <xnamath.h>
#include <DxErr.h>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include "MathHelper.h"
#include "LightHelper.h"

//���������//
#if defined(DEBUG)|defined(_DEBUG)
	#ifndef HR
	#define HR(x)
	{
		HRESULT hr=(x);//x��������ֵ��������ΪHRESULT��С��0ʧ��//
		if(FAILED(hr))
		{
			DXTrace(__FILE__,__LINE__,hr,L#x,true);//��ǰ�ļ�ͨ��Ϊ__FILE__����ǰ��__LINE__,һ���������ٵ���������HRESULT,һ���������ٵ��������Ŀ��ַ����������ǿգ�ͨ��L#ת��Ϊ���ַ�����bool�Ƿ񵯳���Ϣ��

		}

	}
	#endif
#else
#ifndef HR
#define HR(x)(x)
#endif
#endif//DEBUG

#endif//D3DUTIL_H
//����һ���ͷ����COM�����ָ�����ĺ�//���
#define ReleaseCOM(x){if(x){x->Release();x=0;}}
//����ɾ��ָ�����ĺ�//ɾ��
#define SafeDelete(x){delete x;x=0;}
//���幤����//


