#pragma once
class GameTimer
{
public:
	GameTimer();
	//~GameTimer(void);
	float TotalTime() const;
	float DeltaTime() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();
private:
	double mSecondsPerCount;//每秒计时次数
	double mDeltaTime;

	__int64 mBaseTime;//各个编译器对int4支持不一样，__int64是win支持的64longlong类型
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;//上一帧时间
	__int64 mCurrTime;

	bool mStopped;



};

