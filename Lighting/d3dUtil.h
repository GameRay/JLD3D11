#ifndef  D3DUTIL_H
#define  D3DUTIL_H

#if defined(DEBUG)||defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include<crtdbg.h>////1.使用_PRTn的宏显示调试信息 2.侦测内存泄漏用_CrtSetDbgFlag函数（本例是用这个）
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

//定义错误检测//
#if defined(DEBUG)|defined(_DEBUG)
	#ifndef HR
	#define HR(x)
	{
		HRESULT hr=(x);//x函数返回值返回类型为HRESULT，小于0失败//
		if(FAILED(hr))
		{
			DXTrace(__FILE__,__LINE__,hr,L#x,true);//当前文件通常为__FILE__，当前行__LINE__,一个将被跟踪到调试流的HRESULT,一个将被跟踪到调试流的宽字符串（可能是空）通过L#转化为宽字符串，bool是否弹出消息框

		}

	}
	#endif
#else
#ifndef HR
#define HR(x)(x)
#endif
#endif//DEBUG

#endif//D3DUTIL_H
//定义一个释放清空COM组件的指针对象的宏//清空
#define ReleaseCOM(x){if(x){x->Release();x=0;}}
//定义删除指针对象的宏//删除
#define SafeDelete(x){delete x;x=0;}
//定义工具类//


