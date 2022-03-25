#include "Timer.h"
#include <SDL2/SDL.h>
uint32_t Timer::mStartTick = 0;
uint32_t Timer::mCurTick = 0;
uint32_t Timer::mLastTick = 0;
float Timer::mDeltaTime = 0.0f;

Timer::Timer()
{
}

void Timer::Init()
{
	mStartTick = SDL_GetTicks();
}

void Timer::Update(uint32_t lockFrame)
{
	mCurTick = SDL_GetTicks();
	if (lockFrame > 0)
		while (!SDL_TICKS_PASSED(SDL_GetTicks(), mCurTick + (1.0f / lockFrame) * 1000))
			;

	mDeltaTime = (mCurTick - mLastTick) / 1000.0f;
	mLastTick = mCurTick;
	if (mDeltaTime < 0.05f)
		mDeltaTime = 0.05f;
}

float Timer::GetDeltaTime()
{
	return mDeltaTime;
}
