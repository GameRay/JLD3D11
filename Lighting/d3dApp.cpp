#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>
namespace
{
	//这仅用于将Windows消息从全局窗口过程转发到我们的成员函数窗口过程，因为我们不能将成员函数直接分配给WNDCLASS :: lpfnWndProc。//
	D3DApp*gd3dApp=0;//指向此类对象本身////创建的而当前类对象，hInstance是为其分配的标识唯一性的句柄。//
}
//回调函数,注册给函数指针，之后进行调用（可以理解类似用于事件注册的函数）//
LRESULT CALLBACK
MainWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	return gd3dApp->MsgProc(hwnd,msg,wParam,lParam);

}
LRESULT D3DApp::MsgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch(msg)
	{
		//窗口激活类消息//
	case WM_ACTIVATE:
		//WA_CLICKACTIVE       通过鼠标单击激活了该窗口 
		//WA_ACTIVE                   通过鼠标以外的工具（如键盘）激活了该窗口 
		//WA_INACTIVE               取消该窗口的激活 
		//该信息保存在 wParam 的低阶字中//
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
		//窗口尺寸发生变化的消息//
	case WM_SIZE:
		//此时窗口长宽信息分别被存储在lParam的高位和地位字中//
		//WPARAM指出窗口新的状态//
		mClientWidth=LOWORD(lParam);
		mClientHeight=HIWORD(lParam);
		if(md3dDevice)
		{
			//触发最大/最小化按钮，处于最小状态时//
			if(wParam==SIZE_MINIMIZED)
			{
				mAppPaused=true;
				mMinimized=true;
				mMaximized=false;

			}
			//触发最大/最小化按钮，处于最大状态时//
			else if(wParam == SIZE_MAXIMIZED)
			{

				mAppPaused=false;
				mMinimized=false;
				mMaximized=true;
				OnResize();
			}
			else if(wParam==SIZE_RESTORED)
			{
				//最大化会变成全屏，最小化会变成窗口下面缩略图图标//
				if(mMinimized)//从最小状态恢复变大起始时，将最小状态标志位置为false,变化过程中调整大小//
				{
					mAppPaused=false;
					mMinimized=false;
					OnResize();

				}
				else if (mMaximized)//从最大状态恢复变小起始时，将最大状态标志位置为false,变化过程中调整大小//
				{
					mAppPaused=false;
					mMaximized=false;
					OnResize();
				}
				else if (mReSizing)//通过菜单触发的的最小到最大变化的过程中//
				{

				}
				else//非菜单触发,非拖拽这些带过程的窗口大小变化情景，api函数直接改变窗口打小的情况//
				{
					OnResize();
				}

			}


		}
		return 0;
	case WM_ENTERSIZEMOVE://进入移动窗口的状态
		mAppPaused=true;
		mReSizing=true;
		mTimer.Stop();
		return 0;
	case WM_EXITSIZEMOVE://退出拖动窗口状态/./
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
		//Accelerator key比较容易理解，字面意思也很简单，快捷键，就是通过键盘的几个键组合，触发一个action的执行。 

		//Mnemonic key通常是菜单，按钮，文本标签等控件的文本中带下划线得一个字符，用来提示用户可以同时按下Alt键和这个字符对应得键来触发相应的动作，比如触发对应按钮的命令执行。
		//用户按下快捷键或助记键是触发次消息//
		// 如果使用者按下Alt和一个与菜单项不匹配的字符时，或者在显示弹出式菜单而  
		// 使用者按下一个与弹出式菜单里的项目不匹配的字符键时。   
	case WM_MENUCHAR:
		// 按下alt-enter切换全屏时不发出声响  
		return MAKELRESULT(0,MNC_CLOSE);//低位和高位合成一个双字

		//捕获此消息，防止窗口变得过小,比如拉伸的过小,指定最小尺寸//
	case WM_GETMINMAXINFO:
		((MINMAXINFO*)lParam)->ptMinTrackSize.x=200;
		((MINMAXINFO*)lParam)->ptMaxTrackSize.y=200;
		return 0;
	//处理鼠标按下事件//
	case WM_LBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONDOWN:
		 OnMouseDown(wParam,GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
		 return 0;
	//处理鼠标松开事件//	
	case WM_LBUTTONUP:
	case WM_MBUTTONUP:
	case WM_RBUTTONUP:
		OnMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;
		//处理鼠标移动事件//
	case WM_MOUSEMOVE:
		OnMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;


	}
	
	return DefWindowProc(hwnd,msg,wParam,lParam);//DefWindowProc这个函数是默认的窗口处理函数，我们可以把不关心的消息都丢给它来处理

}

bool D3DApp::InitMainWindow()
{

	//对应用程序的窗口对象创建
	WNDCLASS wc;
	wc.style=CS_HREDRAW|CS_VREDRAW;//CS_HREDRAW  ,CS_VREDRAW 的意思：CS_HREDRAW：一旦移动或尺寸调整使客户区的宽度发生变化，就重新绘制窗口； ⑵CS_VREDRAW：一旦移动或尺寸调整使客户区的高度发生变化，就重新绘制窗口。
	wc.lpfnWndProc=MainWndProc;
	//cbClsExtra是对类的（这里所说的类不是C++中的类，而是WNDCLASS结构的变量用RegisterClass注册后，在系统中记录的一组信息，是窗口的一个模板），用该类生成的所有窗口共享该附加空间。类似于C++类中的static变量。
	//cbWndExtra是对窗口的，每实例化一个窗口都有这么一个附加空间。类似于C++类中的成员变量。
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=mhAppInst;
	wc.hIcon=LoadIcon(0,IDI_APPLICATION);
	wc.hCursor=LoadCursor(0,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName=0;
	wc.lpszClassName=L"D3DWndClassName";
	//对程序窗口进行注册//
	if(!RegisterClass(&wc))
	{

		MessageBox(0,L"egisterClass Failed.",0,0);//窗口，显示内容，标题，类型//
		return false;
	}
	//根据请求的客户显示区域大小计算出窗口的大小//
	RECT R={0,0,mClientWidth,mClientHeight};
	AdjustWindowRect(&R,WS_OVERLAPPEDWINDOW,false);//该函数依据所需客户矩形的大小，计算需要的窗口矩形的大小
	//lpRect：指向RECT结构的指针，该结构包含所需客户区域的左上角和右下角的坐标。函数返回时，该结构容纳所需客户区域的窗口的左上角和右下角的坐标。
	//dwStyle：指定将被计算尺寸的窗口的窗口风格。
	//bMenu：指示窗口是否有菜单。如窗口有菜单，则设为0x1/*_TRUE*/（非零）
	int width=R.right-R.left;
	int	heigh=R.bottom-R.top;

	//创建窗口//
	//（窗口类名\窗口标题\窗口风格（有无边框等）\窗口起始位置x\窗口起始位置y\窗口宽\窗口高\父窗口（可选）\菜单句柄\与窗口关联的模块实例的句柄\传递窗口WM_CREATE消息的参数）//
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