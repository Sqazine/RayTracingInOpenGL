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
	const uint8_t *m_CurKeyState;
	uint8_t *m_PreKeyState;
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
	bool m_IsRelative;
	Vector2i32 m_CurPos;
	Vector2i32 m_PrePos;
	Vector2i32 m_MouseScrollWheel;
	uint32_t m_CurButtons;
	uint32_t m_PreButtons;
};

class Input
{
public:
	static const std::shared_ptr<KeyboardState> &GetKeyboard();
	static const std::shared_ptr<MouseState> &GetMouse();

private:
	friend class Application;
	static void Init();
	static void PreUpdate();
	static void PostUpdate();
	static void ProcessInput(SDL_Event event);

private:
	static std::shared_ptr<KeyboardState> m_KeyboardState;
	static std::shared_ptr<MouseState> m_MouseState;
};