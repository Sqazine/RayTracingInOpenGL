#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include <memory>
#include "Vector2.h"

enum class ButtonState
{
	NONE,
	PRESS,
	RELEASE,
	HOLD
};

class KeyboardState
{
public:
	KeyboardState();
	~KeyboardState();
	bool GetKeyValue(SDL_Scancode keyCode) const;
	ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	friend class Input;
	const uint8_t *mCurKeyState;
	uint8_t *mPreKeyState;
};

class MouseState
{
public:
	MouseState();
	~MouseState();
	bool GetButtonValue(int button) const;
	ButtonState GetButtonState(int button) const;
	Vector2i32 GetMousePos() const;
	Vector2i32 GetReleativeMove() const;
	Vector2i32 GetMouseScrollWheel() const;

	void SetReleativeMode(bool isActive);
	bool IsReleativeMode() const;

private:
	friend class Input;
	bool mIsRelative;
	Vector2i32 mCurPos;
	Vector2i32 mPrePos;
	Vector2i32 mMouseScrollWheel;
	uint32_t mCurButtons;
	uint32_t mPreButtons;
};

class Input
{
public:
	static const std::shared_ptr<KeyboardState> &GetKeyboard();
	static const std::shared_ptr<MouseState> &GetMouse();

private:
	friend class App;
	static void Init();
	static void PreUpdate();
	static void PostUpdate();
	static void ProcessInput(SDL_Event event);

private:
	static std::shared_ptr<KeyboardState> mKeyboardState;
	static std::shared_ptr<MouseState> mMouseState;
};