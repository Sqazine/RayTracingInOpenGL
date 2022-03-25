#include "Input.h"
#include <memory>

KeyboardState::KeyboardState()
{
}
KeyboardState::~KeyboardState()
{
}
bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return mCurKeyState[keyCode] == 1 ? true : false;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (mPreKeyState[keyCode] == 0)
	{
		if (mCurKeyState[keyCode] == 0)
			return ButtonState::NONE;
		else
			return ButtonState::PRESS;
	}
	else
	{
		if (mCurKeyState[keyCode] == 0)
			return ButtonState::RELEASE;
		else
			return ButtonState::HOLD;
	}
}

MouseState::MouseState()
	: mCurPos(Vector2i32::ZERO), mPrePos(Vector2i32::ZERO), mMouseScrollWheel(Vector2i32::ZERO), mCurButtons(0), mPreButtons(0)
{
}
MouseState::~MouseState()
{
}
bool MouseState::GetButtonValue(int button) const
{
	return (mCurButtons & SDL_BUTTON(button)) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	if ((mPreButtons & SDL_BUTTON(button)) == 0)
	{
		if ((mPreButtons & SDL_BUTTON(button)) == 0)
			return ButtonState::NONE;
		else
			return ButtonState::PRESS;
	}
	else
	{
		if ((mPreButtons & SDL_BUTTON(button)) == 0)
			return ButtonState::RELEASE;
		else
			return ButtonState::HOLD;
	}
}

Vector2i32 MouseState::GetMousePos() const
{
	return mCurPos;
}

Vector2i32 MouseState::GetReleativeMove() const
{
	return mCurPos - mPrePos;
}

Vector2i32 MouseState::GetMouseScrollWheel() const
{
	return mMouseScrollWheel;
}

void MouseState::SetReleativeMode(bool isActive)
{
	mIsRelative = isActive;
	if (isActive)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

bool MouseState::IsReleativeMode() const
{
	return mIsRelative;
}

std::shared_ptr<KeyboardState> Input::mKeyboardState = nullptr;
std::shared_ptr<MouseState> Input::mMouseState = nullptr;

void Input::Init()
{
	mKeyboardState = std::make_shared<KeyboardState>();
	mMouseState = std::make_shared<MouseState>();
	//get keyboard state
	mKeyboardState->mCurKeyState = SDL_GetKeyboardState(nullptr);
	//clear previous keyboard state
	mKeyboardState->mPreKeyState = new uint8_t[SDL_NUM_SCANCODES];
	memset(mKeyboardState->mPreKeyState, 0, SDL_NUM_SCANCODES);
}

void Input::PreUpdate()
{
	memcpy_s(mKeyboardState->mPreKeyState, SDL_NUM_SCANCODES, mKeyboardState->mCurKeyState, SDL_NUM_SCANCODES);
	mMouseState->mPreButtons = mMouseState->mCurButtons;
	mMouseState->mPrePos = mMouseState->mCurPos;
	mMouseState->mMouseScrollWheel = Vector2i32::ZERO;
}

void Input::PostUpdate()
{
	Vector2i32 p = Vector2i32::ZERO;

	if (!mMouseState->mIsRelative) //get mouse cursor absolute pos
		mMouseState->mCurButtons = SDL_GetMouseState(&p.x, &p.y);
	else //get mouse cursor relative pos
		mMouseState->mCurButtons = SDL_GetRelativeMouseState(&p.x, &p.y);

	mMouseState->mCurPos = p;
}

void Input::ProcessInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		mMouseState->mMouseScrollWheel = Vector2i32(event.wheel.x, static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}
}

const std::shared_ptr<KeyboardState> &Input::GetKeyboard()
{
	return mKeyboardState;
}

const std::shared_ptr<MouseState> &Input::GetMouse()
{
	return mMouseState;
}
