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

	static uint32_t m_StartTick;
	static uint32_t m_CurTick;
	static uint32_t m_LastTick;
	static float m_DeltaTime;
};