#include "Framework.h"
#include "Timer.h"

#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

Timer::Timer() :
	frameCount(0)
	, fps(0)
	, maxFPS(0)
	, limitFPS(200)
	, oneSecCount(0)
	, runningTime(0)
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
}

Timer::~Timer() = default;

void Timer::Update()
{
	QueryPerformanceCounter(&endTime);
	timeElapsed = (double)(endTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart;

	// 경과된 시간이 원하는 프레임 시간보다 짧으면
	double minFrameTime = GetMinFrameTime();
	if (timeElapsed < minFrameTime)
	{
		auto sleepTime = (DWORD)((minFrameTime - timeElapsed) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		
		// 일정시간 지난 후 timeElapsed를 다시 측정해준다.
		QueryPerformanceCounter(&endTime);
		timeElapsed = (double)(endTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart;
	}
#if _DEBUG
	if (timeElapsed > GetMaxFrameTime())
	{
		timeElapsed = GetMaxFrameTime();
	}
#endif // 프레임 시간이 너무 길면 최소 30fps로 만들어준다 (Debug용)
	
	startTime = endTime;
	oneSecCount += timeElapsed; // 경과시간 누적

	++frameCount;
	if (oneSecCount >= 1.0)
	{
		oneSecCount = 0;
		fps = frameCount;
		frameCount = 0;
	}

	runningTime += timeElapsed;
}
