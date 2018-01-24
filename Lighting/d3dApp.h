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
	//框架方式，驱动客户类重载这些方法来实现指定的应用需求//
	virtual bool Init();
	virtual void OnResize();
	virtual void UpdateScene(float dt)=0;
	virtual void DrawScene()=0;
	virtual LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	//方便重载鼠标句柄的输入//

	virtual void OnMouseDown(WPARAM btnState, int x, int y){};
	virtual void OnMouseUp(WPARAM btnState, int x, int y){};
	virtual void OnMouseMove(WPARAM btnState, int x, int y){};

protected:
	bool InitMainWindow();
	bool InitDirect3D();

	void CalculateFrameStats();

protected:
	HINSTANCE mhAppInst;//应用程序实例句柄///双击QQ快捷方式, 会启动一个qq程序, 假设给它个编号9527;
	//再双击QQ快捷方式, 又启动了一个新的qq, 假设编号9528;，
	//对于9528来说, hInstance=9528，这个值从WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)传过来,创建完类对象实例后取得值HINSTANCE，再将其传递给HINSTANCE注册窗口函数//
	HWND mhMainWnd;//主窗口句柄//
	bool mAppPaused;//是否处于暂停状态//
	bool mMinimized;//窗口最小化状态//
	bool mMaximized;//窗口最大化状态//
	bool mReSizing;//窗口尺寸变化中//
	bool m4xMsaaQuality;//4X MSAA质量等级//

	GameTimer mTimer;//游戏时间类//

	ID3D11Device* md3dDevice;//设备//
	ID3D11DeviceContext*md3dImmediateContext;//呈现内容//
	IDXGISwapChain* mSwapChain;//
	ID3D11Texture2D* mDepthStencilBuffer;//深度模板缓存//
	ID3D11DepthStencilView*mDepthStencilView;//深度模板缓存视图//
	ID3D11RenderTargetView*mRenderTargetView;//渲染目标视图//
	D3D11_VIEWPORT mScreenViewport;//视口//

	//自类在构造函数里自定义这些值//
	std::wstring mMainWndCaption;//窗口标题//
	D3D_DRIVER_TYPE md3dDriverType;//硬件设备还是引用设备类型//

	//初始窗口大小//
	int mClientHeight;
	int mClientWidth;
	//是否使用4x多重采样//
	bool mEnable4xMsaa;
};

#endif