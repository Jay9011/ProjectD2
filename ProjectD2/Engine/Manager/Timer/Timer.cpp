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

	// ����� �ð��� ���ϴ� ������ �ð����� ª����
	double minFrameTime = GetMinFrameTime();
	if (timeElapsed < minFrameTime)
	{
		auto sleepTime = (DWORD)((minFrameTime - timeElapsed) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		
		// �����ð� ���� �� timeElapsed�� �ٽ� �������ش�.
		QueryPerformanceCounter(&endTime);
		timeElapsed = (double)(endTime.QuadPart - startTime.QuadPart) / (double)frequency.QuadPart;
	}
#if _DEBUG
	if (timeElapsed > GetMaxFrameTime())
	{
		timeElapsed = GetMaxFrameTime();
	}
#endif // ������ �ð��� �ʹ� ��� �ּ� 30fps�� ������ش� (Debug��)
	
	startTime = endTime;
	oneSecCount += timeElapsed; // ����ð� ����

	++frameCount;
	if (oneSecCount >= 1.0)
	{
		oneSecCount = 0;
		fps = frameCount;
		frameCount = 0;
	}

	runningTime += timeElapsed;
}
