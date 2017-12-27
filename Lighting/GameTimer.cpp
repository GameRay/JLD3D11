#include <Windows.h>//查询计时器函数包含在此
#include "GameTimer.h"


GameTimer::GameTimer()
	:mSecondsPerCount(0.0),mDeltaTime(-1.0),mBaseTime(0),
	mPausedTime(0),mPrevTime(0),mCurrTime(0),mStopped(false)
{
	__int64 countPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countPerSec);
	mSecondsPerCount=1.0/(double)countPerSec;
}

float GameTimer::TotalTime() const
{
	//当前处于暂停状态//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime
	//  启动时的时刻 开始停止的时间		 暂停切换到开始的时刻
	if (mStopped)
	{

		return(float)(mStopTime-mPausedTime-mBaseTime)*mSecondsPerCount;

	}
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<--paused time-->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime
	//非暂停状态
	else
	{

		return(float)(mCurrTime-mPausedTime-mBaseTime)*mSecondsPerCount;

	}

}

float GameTimer::DeltaTime() const
{
	return(float)mDeltaTime;
}

float GameTimer::Reset()
{

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseTime=currTime;
	mPrevTime=currTime;
	mStopTime=0;
	mStopped=false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if(mStopped)
	{
		mPausedTime+=(startTime-mStopTime);
		mPrevTime=startTime;
		mStopTime=0;
		mStopped=false;

	}

}

void GameTimer::Stop()
{
	if (!mStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime=currTime;
		mStopped=true;
	}
		
}
void GameTimer::Tick()
{

	if(mStopped)
	{
		mDeltaTime=0.0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime=currTime;

	mDeltaTime=(mCurrTime-mPrevTime)*mSecondsPerCount;

	mPrevTime=mCurrTime;//
	//省电模式或者切换处理器计算出来的mDeltaTime可能是负的，这里强制非负。
	if(mDeltaTime<0.0)
	{

		mDeltaTime=0.0;
	}
}