#ifndef D3DUTIL_H
#define D3DUTIL_H

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>//1.使用_PRTn的宏显示调试信息 2.侦测内存泄漏用_CrtSetDbgFlag函数（本例是用这个）
#endif

#include <d3dx11.h>
#include "d3dx11Effect.h"
#include <xnamath.h>
#include <dxerr.h>
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

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) 
	{
		HRESULT hr = (x);//x函数返回值返回类型为HRESULT，小于0失败//
		if(FAILED(hr))
		{
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);//当前文件通常为__FILE__，当前行__LINE__,一个将被跟踪到调试流的HRESULT,一个将被跟踪到调试流的宽字符串（可能是空）通过L#转化为宽字符串，bool是否弹出消息框//

		}
	};
#endif
#else
#ifndef HR
#define HR(x)(x)
#endif
#endif
//DEBUG


//定义一个释放清空COM组件的指针对象的宏//清空
#define ReleaseCOM(x){if(x){x->Release();x=0;}}
//定义删除指针对象的宏//删除
#define SafeDelete(x){delete x;x=0;}
//定义工具类//

class d3dHelper
{

public:
	//SRV只读，RTV可写//
	static ID3D11ShaderResourceView* CreateTexture2DArraySRV(
		ID3D11Device*device,ID3D11DeviceContext*context,
		std::vector<std::wstring>&filenames,
		DXGI_FORMAT format=DXGI_FORMAT_FROM_FILE,
		UINT filter=D3DX11_FILTER_NONE,
		UINT mipFilter=D3DX11_FILTER_LINEAR
		);
	static ID3D11ShaderResourceView*CreateRandomTexture1DSRV(ID3D11Device*device);
};


class TextHelper
{

public:
	//T转化为宽字符
	template<typename T>
	static D3DX11INLINE std::wstring ToString(const T&s)
	{

		std::wostringstream oss;
		oss<<s;
		return oss.str();
	}
	//宽字符转化为T
	template<typename T>
	static D3DX11INLINE T FromString(const std::wstring&s)
	{

		T x;
		std::wistringstream iss(s)；
		iss>>x;
		return x;
	}
};
//提取平截头体的四个面//
void ExtractFrustumPlanes(XMFLOAT4 planes[6], CXMMATRIX M);


namespace Colors
{
	XMGLOBALCONST XMVECTORF32 White     = {1.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Black     = {0.0f, 0.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Red       = {1.0f, 0.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Green     = {0.0f, 1.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Blue      = {0.0f, 0.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Yellow    = {1.0f, 1.0f, 0.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Cyan      = {0.0f, 1.0f, 1.0f, 1.0f};
	XMGLOBALCONST XMVECTORF32 Magenta   = {1.0f, 0.0f, 1.0f, 1.0f};

	XMGLOBALCONST XMVECTORF32 Silver    = {0.75f, 0.75f, 0.75f, 1.0f};
	XMGLOBALCONST XMVECTORF32 LightSteelBlue = {0.69f, 0.77f, 0.87f, 1.0f};
}

//
//types和formats进行转换的工具类//
//

class Convert
{

public:
	//XMVECTOR to XMCOLOR//
	static D3DX11INLINE XMCOLOR ToXmColor(FXMVECTOR v)
	{

		XMCOLOR dest;
		XMStoreColor(&dest,v);
		return dest;
	}
	//  XMVECTOR to XMFLOAT4//
	static D3DX11INLINE XMFLOAT4 ToXmFloat4(FXMVECTOR v)
	{

		XMFLOAT4 dest;
		XMStoreFloat4(&dest,v);
		return dest;
	}

	//argb to abgr //

	static D3DX11INLINE UINT ArgbToAbgr(UINT argb)
	{
		BYTE A=(argb>>24)&0xff;
		BYTE R=(argb>>16)&0xff;
		BYTE G=(argb>>8)&0xff;
		BYTE B=(argb>>0)&0xff;

		return(A<<24)|(B<<16)|(G<<8)|(R<<0);
	}
};
#endif//D3DUTIL_H
