#include "d3dApp.h"
#include <WindowsX.h>
#include <sstream>
namespace
{
	//这仅用于将Windows消息从全局窗口过程转发到我们的成员函数窗口过程，因为我们不能将成员函数直接分配给WNDCLASS :: lpfnWndProc。//
	D3DApp*gd3dApp=0;//指向此类对象本身//
}
