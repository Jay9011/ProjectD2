#pragma once
class Timer
{
#pragma region 생성자 소멸자
public:
	Timer();
	virtual ~Timer();
#pragma endregion
#pragma region private Member
private:
	LARGE_INTEGER startTime{};
	LARGE_INTEGER endTime{};
	LARGE_INTEGER frequency{};
	
	double timeElapsed;	// 프레임 사이의 [경과] 시간
	
	float frameCount;
	float fps;
	float maxFPS;

	double oneSecCount;
	double runningTime;
#pragma endregion

public:
	void Update();
	
	/* === === === === ===
	*	Getter/Setter
	*  === === === === === */
	double Delta()           const { return timeElapsed; }
	float  FPS()             const { return fps; }
	double RunningTime()     const { return runningTime; }
	double GetMinFrameTime() const { if (maxFPS !=0) return 1.0 / maxFPS; else return 0.0; }
	double GetMaxFrameTime() const { return 1.0 / 30.0; }	// 디버깅용 최소 30fps는 나와야 한다.
	float  GetMaxFPS()       const { return maxFPS; }
	void   SetMaxFPS(float _fps)   { maxFPS = _fps; }
	
};
