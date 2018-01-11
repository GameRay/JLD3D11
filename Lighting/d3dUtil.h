#ifndef D3DUTIL_H
#define D3DUTIL_H

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>//1.ʹ��_PRTn�ĺ���ʾ������Ϣ 2.����ڴ�й©��_CrtSetDbgFlag�������������������
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
//���������//

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x) 
	{
		HRESULT hr = (x);//x��������ֵ��������ΪHRESULT��С��0ʧ��//
		if(FAILED(hr))
		{
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true);//��ǰ�ļ�ͨ��Ϊ__FILE__����ǰ��__LINE__,һ���������ٵ���������HRESULT,һ���������ٵ��������Ŀ��ַ����������ǿգ�ͨ��L#ת��Ϊ���ַ�����bool�Ƿ񵯳���Ϣ��//

		}
	};
#endif
#else
#ifndef HR
#define HR(x)(x)
#endif
#endif
//DEBUG


//����һ���ͷ����COM�����ָ�����ĺ�//���
#define ReleaseCOM(x){if(x){x->Release();x=0;}}
//����ɾ��ָ�����ĺ�//ɾ��
#define SafeDelete(x){delete x;x=0;}
//���幤����//

class d3dHelper
{

public:
	//SRVֻ����RTV��д//
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
	//Tת��Ϊ���ַ�
	template<typename T>
	static D3DX11INLINE std::wstring ToString(const T&s)
	{

		std::wostringstream oss;
		oss<<s;
		return oss.str();
	}
	//���ַ�ת��ΪT
	template<typename T>
	static D3DX11INLINE T FromString(const std::wstring&s)
	{

		T x;
		std::wistringstream iss(s)��
		iss>>x;
		return x;
	}
};
//��ȡƽ��ͷ����ĸ���//
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
//types��formats����ת���Ĺ�����//
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
