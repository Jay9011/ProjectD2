#pragma once
#define DT		Core::Get()->GetTimer()->Delta()
class Timer
{
#pragma region ������ �Ҹ���
public:
	Timer();
	virtual ~Timer();
#pragma endregion
#pragma region private Member
private:
	LARGE_INTEGER startTime{};
	LARGE_INTEGER endTime{};
	LARGE_INTEGER frequency{};
	
	double timeElapsed;	// ������ ������ [���] �ð�
	
	float frameCount;
	float fps;
	float maxFPS;
	float limitFPS;

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
	double GetMinFrameTime() const { if (maxFPS != 0) return (maxFPS > limitFPS) ? (1.0 / limitFPS) : (1.0 / maxFPS); else return (1.0 / limitFPS); }
	double GetMaxFrameTime() const { return 1.0 / 30.0; }	// ������ �ּ� 30fps�� ���;� �Ѵ�.
	float  GetMaxFPS()       const { return maxFPS; }
	void   SetMaxFPS(float _fps)   { maxFPS = _fps; }
	
};
