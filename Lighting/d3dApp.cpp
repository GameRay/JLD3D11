#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>
namespace
{
	//������ڽ�Windows��Ϣ��ȫ�ִ��ڹ���ת�������ǵĳ�Ա�������ڹ��̣���Ϊ���ǲ��ܽ���Ա����ֱ�ӷ����WNDCLASS :: lpfnWndProc��//
	D3DApp*gd3dApp=0;//ָ����������////�����Ķ���ǰ�����hInstance��Ϊ�����ı�ʶΨһ�Եľ����//
}
//�ص�����,ע�������ָ�룬֮����е��ã�����������������¼�ע��ĺ�����//
LRESULT CALLBACK
MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	return gd3dApp->MsgProc(hwnd,msg,wParam,lParam);

}
LRESULT D3DApp::MsgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		//���ڼ�������Ϣ//
	case WM_ACTIVATE:
		//WA_CLICKACTIVE       ͨ����굥�������˸ô��� 
		//WA_ACTIVE                   ͨ���������Ĺ��ߣ�����̣������˸ô��� 
		//WA_INACTIVE               ȡ���ô��ڵļ��� 
		//����Ϣ������ wParam �ĵͽ�����//
			if (LOWORD(wParam)==WA_INACTIVE)
			{
				mAppPaused=true;
				mTimer.Stop();
			}
			else
			{

				mAppPaused=false;
				mTimer.Start();
			}
			return 0;
		//���ڳߴ緢���仯����Ϣ//
	case WM_SIZE:
		//��ʱ���ڳ�����Ϣ�ֱ𱻴洢��lParam�ĸ�λ�͵�λ����//
		//WPARAMָ�������µ�״̬//
		mClientWidth=LOWORD(lParam);
		mClientHeight=HIWORD(lParam);
		if(md3dDevice)
		{
			//�������/��С����ť��������С״̬ʱ//
			if(wParam==SIZE_MINIMIZED)
			{
				mAppPaused=true;
				mMinimized=true;
				mMaximized=false;

			}
			//�������/��С����ť���������״̬ʱ//
			else if(wParam == SIZE_MAXIMIZED)
			{

				mAppPaused=false;
				mMinimized=false;
				mMaximized=true;
				OnResize();
			}
			else if(wParam==SIZE_RESTORED)
			{
				//��󻯻���ȫ������С�����ɴ�����������ͼͼ��//
				if(mMinimized)//����С״̬�ָ������ʼʱ������С״̬��־λ��Ϊfalse,�仯�����е�����С//
				{
					mAppPaused=false;
					mMinimized=false;
					OnResize();

				}
				else if (mMaximized)//�����״̬�ָ���С��ʼʱ�������״̬��־λ��Ϊfalse,�仯�����е�����С//
				{
					mAppPaused=false;
					mMaximized=false;
					OnResize();
				}
				else if (mReSizing)//ͨ���˵������ĵ���С�����仯�Ĺ�����//
				{

				}
				else//�ǲ˵�����,����ק��Щ�����̵Ĵ��ڴ�С�仯�龰��api����ֱ�Ӹı䴰�ڴ�С�����//
				{
					OnResize();
				}

			}


		}
		return 0;
	case WM_ENTERSIZEMOVE://�����ƶ����ڵ�״̬
		mAppPaused=true;
		mReSizing=true;
		mTimer.Stop();
		return 0;
	case WM_EXITSIZEMOVE://�˳��϶�����״̬/./
		mAppPaused=false;
		mReSizing=false;
		mTimer.Start();
		OnResize();
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		// The WM_MENUCHAR message is sent when a menu is active and the user presses 
		// a key that does not correspond to any mnemonic or accelerator key. 
		//Accelerator key�Ƚ�������⣬������˼Ҳ�ܼ򵥣���ݼ�������ͨ�����̵ļ�������ϣ�����һ��action��ִ�С� 

		//Mnemonic keyͨ���ǲ˵�����ť���ı���ǩ�ȿؼ����ı��д��»��ߵ�һ���ַ���������ʾ�û�����ͬʱ����Alt��������ַ���Ӧ�ü���������Ӧ�Ķ��������紥����Ӧ��ť������ִ�С�
		//�û����¿�ݼ������Ǽ��Ǵ�������Ϣ//
		// ���ʹ���߰���Alt��һ����˵��ƥ����ַ�ʱ����������ʾ����ʽ�˵���  
		// ʹ���߰���һ���뵯��ʽ�˵������Ŀ��ƥ����ַ���ʱ��   
	case WM_MENUCHAR:
		// ����alt-enter�л�ȫ��ʱ����������  
		return MAKELRESULT(0,MNC_CLOSE);//��λ�͸�λ�ϳ�һ��˫��

		//�������Ϣ����ֹ���ڱ�ù�С,��������Ĺ�С,ָ����С�ߴ�//
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x=200;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y=200;
		return 0;
	//������갴���¼�//
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		 OnMouseDown(wParam,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		 return 0;
	//��������ɿ��¼�//	
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
		//��������ƶ��¼�//
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;


	}
	
	return DefWindowProc(hwnd,msg,wParam,lParam);//DefWindowProc���������Ĭ�ϵĴ��ڴ����������ǿ��԰Ѳ����ĵ���Ϣ��������������

}

bool D3DApp::InitMainWindow()
{

	//��Ӧ�ó���Ĵ��ڶ��󴴽�
	WNDCLASS wc;
	wc.style=CS_HREDRAW|CS_VREDRAW;//CS_HREDRAW  ,CS_VREDRAW ����˼��CS_HREDRAW��һ���ƶ���ߴ����ʹ�ͻ����Ŀ�ȷ����仯�������»��ƴ��ڣ� ��CS_VREDRAW��һ���ƶ���ߴ����ʹ�ͻ����ĸ߶ȷ����仯�������»��ƴ��ڡ�
	wc.lpfnWndProc=MainWndProc;
	//cbClsExtra�Ƕ���ģ�������˵���಻��C++�е��࣬����WNDCLASS�ṹ�ı�����RegisterClassע�����ϵͳ�м�¼��һ����Ϣ���Ǵ��ڵ�һ��ģ�壩���ø������ɵ����д��ڹ���ø��ӿռ䡣������C++���е�static������
	//cbWndExtra�ǶԴ��ڵģ�ÿʵ����һ�����ڶ�����ôһ�����ӿռ䡣������C++���еĳ�Ա������
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=mhAppInst;
	wc.hIcon=LoadIcon(0,IDI_APPLICATION);
	wc.hCursor=LoadCursor(0,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName=0;
	wc.lpszClassName=L"D3DWndClassName";
	//�Գ��򴰿ڽ���ע��//
	if(!RegisterClass(&wc))
	{

		MessageBox(0,L"egisterClass Failed.",0,0);//���ڣ���ʾ���ݣ����⣬����//
		return false;
	}
	//��������Ŀͻ���ʾ�����С��������ڵĴ�С//
	RECT R={0,0,mClientWidth,mClientHeight};
	AdjustWindowRect(&R,WS_OVERLAPPEDWINDOW,false);//�ú�����������ͻ����εĴ�С��������Ҫ�Ĵ��ھ��εĴ�С
	//lpRect��ָ��RECT�ṹ��ָ�룬�ýṹ��������ͻ���������ϽǺ����½ǵ����ꡣ��������ʱ���ýṹ��������ͻ�����Ĵ��ڵ����ϽǺ����½ǵ����ꡣ
	//dwStyle��ָ����������ߴ�Ĵ��ڵĴ��ڷ��
	//bMenu��ָʾ�����Ƿ��в˵����細���в˵�������Ϊ0x1/*_TRUE*/�����㣩
	int width=R.right-R.left;
	int	heigh=R.bottom-R.top;

	//��������//
	//����������\���ڱ���\���ڷ�����ޱ߿�ȣ�\������ʼλ��x\������ʼλ��y\���ڿ�\���ڸ�\�����ڣ���ѡ��\�˵����\�봰�ڹ�����ģ��ʵ���ľ��\���ݴ���WM_CREATE��Ϣ�Ĳ�����//
	mhMainWnd=CreateWindow(L"D3DWndClassName",mMainWndCaption.c_str(),WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,width,heigh,0,0,mhAppInst,0);

	if (!mhMainWnd)
	{
		MessageBox(0,L"CreateWindow Failed.",0,0);
		return false;
	}
	ShowWindow(mhMainWnd,SW_SHOW);
	UpdateWindow(mhMainWnd);

	return true;
}