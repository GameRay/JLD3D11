#ifndef D3DAPP_H
#define D3DAPP_H

#include "d3dUtil.h"
#include "GameTimer.h"
#include <string>

class D3DApp
{

public:
	D3DApp(HINSTANCE hInstance);
	virtual~D3DApp();

	HINSTANCE  AppInst() const;
	HWND		MainWnd()const;
	float AspectRatio()const;

	int Run();
	//��ܷ�ʽ�������ͻ���������Щ������ʵ��ָ����Ӧ������//
	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt)=0;
	virtual void DrawScene()=0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//�������������������//

	virtual void OnMouseDown(WPARAM btnState, int x, int y){};
	virtual void OnMouseUp(WPARAM btnState, int x, int y){};
	virtual void OnMouseMove(WPARAM btnState, int x, int y){};

protected:
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStats();

protected:
	HINSTANCE mhAppInst;//Ӧ�ó���ʵ�����///˫��QQ��ݷ�ʽ, ������һ��qq����, ������������9527;
	//��˫��QQ��ݷ�ʽ, ��������һ���µ�qq, ������9528;��
	//����9528��˵, hInstance=9528�����ֵ��WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)������,�����������ʵ����ȡ��ֵHINSTANCE���ٽ��䴫�ݸ�HINSTANCEע�ᴰ�ں���//
	HWND mhMainWnd;//�����ھ��//
	bool mAppPaused;//�Ƿ�����ͣ״̬//
	bool mMinimized;//������С��״̬//
	bool mMaximized;//�������״̬//
	bool mReSizing;//���ڳߴ�仯��//
	bool m4xMsaaQuality;//4X MSAA�����ȼ�//

	GameTimer mTimer;//��Ϸʱ����//

	ID3D11Device* md3dDevice;//�豸//
	ID3D11DeviceContext*md3dImmediateContext;//��������//
	IDXGISwapChain* mSwapChain;//
	ID3D11Texture2D* mDepthStencilBuffer;//���ģ�建��//
	ID3D11DepthStencilView*mDepthStencilView;//���ģ�建����ͼ//
	ID3D11RenderTargetView*mRenderTargetView;//��ȾĿ����ͼ//
	D3D11_VIEWPORT mScreenViewport;//�ӿ�//

	//�����ڹ��캯�����Զ�����Щֵ//
	std::wstring mMainWndCaption;//���ڱ���//
	D3D_DRIVER_TYPE md3dDriverType;//Ӳ���豸���������豸����//

	//��ʼ���ڴ�С//
	int mClientHeight;
	int mClientWidth;
	//�Ƿ�ʹ��4x���ز���//
	bool mEnable4xMsaa;
};

#endif