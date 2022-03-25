#pragma once
#include <cstdint>
class Timer
{
public:
	static void Init();
	static void Update(uint32_t lockFrame = 0);

	static float GetDeltaTime();

private:
	Timer();

	static uint32_t mStartTick;
	static uint32_t mCurTick;
	static uint32_t mLastTick;
	static float mDeltaTime;
};