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
	double mSecondsPerCount;//ÿ���ʱ����
	double mDeltaTime;

	__int64 mBaseTime;//������������int4֧�ֲ�һ����__int64��win֧�ֵ�64longlong����
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;//��һ֡ʱ��
	__int64 mCurrTime;

	bool mStopped;



};

