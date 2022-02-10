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
	return m_CurKeyState[keyCode] == 1 ? true : false;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (m_PreKeyState[keyCode] == 0)
	{
		if (m_CurKeyState[keyCode] == 0)
			return ButtonState::NONE;
		else
			return ButtonState::PRESS;
	}
	else
	{
		if (m_CurKeyState[keyCode] == 0)
			return ButtonState::RELEASE;
		else
			return ButtonState::HOLD;
	}
}

MouseState::MouseState()
	: m_CurPos(Vector2i32::ZERO), m_PrePos(Vector2i32::ZERO), m_MouseScrollWheel(Vector2i32::ZERO), m_CurButtons(0), m_PreButtons(0)
{
}
MouseState::~MouseState()
{
}
bool MouseState::GetButtonValue(int button) const
{
	return (m_CurButtons & SDL_BUTTON(button)) == 1;
}

ButtonState MouseState::GetButtonState(int button) const
{
	if ((m_PreButtons & SDL_BUTTON(button)) == 0)
	{
		if ((m_PreButtons & SDL_BUTTON(button)) == 0)
			return ButtonState::NONE;
		else
			return ButtonState::PRESS;
	}
	else
	{
		if ((m_PreButtons & SDL_BUTTON(button)) == 0)
			return ButtonState::RELEASE;
		else
			return ButtonState::HOLD;
	}
}

Vector2i32 MouseState::GetMousePos() const
{
	return m_CurPos;
}

Vector2i32 MouseState::GetReleativeMove() const
{
	return m_CurPos - m_PrePos;
}

Vector2i32 MouseState::GetMouseScrollWheel() const
{
	return m_MouseScrollWheel;
}

void MouseState::SetReleativeMode(bool isActive)
{
	m_IsRelative = isActive;
	if (isActive)
		SDL_SetRelativeMouseMode(SDL_TRUE);
	else
		SDL_SetRelativeMouseMode(SDL_FALSE);
}

bool MouseState::IsReleativeMode() const
{
	return m_IsRelative;
}

std::shared_ptr<KeyboardState> Input::m_KeyboardState = nullptr;
std::shared_ptr<MouseState> Input::m_MouseState = nullptr;

void Input::Init()
{
	m_KeyboardState = std::make_shared<KeyboardState>();
	m_MouseState = std::make_shared<MouseState>();
	//get keyboard state
	m_KeyboardState->m_CurKeyState = SDL_GetKeyboardState(nullptr);
	//clear previous keyboard state
	m_KeyboardState->m_PreKeyState = new uint8_t[SDL_NUM_SCANCODES];
	memset(m_KeyboardState->m_PreKeyState, 0, SDL_NUM_SCANCODES);
}

void Input::PreUpdate()
{
	memcpy_s(m_KeyboardState->m_PreKeyState, SDL_NUM_SCANCODES, m_KeyboardState->m_CurKeyState, SDL_NUM_SCANCODES);
	m_MouseState->m_PreButtons = m_MouseState->m_CurButtons;
	m_MouseState->m_PrePos = m_MouseState->m_CurPos;
	m_MouseState->m_MouseScrollWheel = Vector2i32::ZERO;
}

void Input::PostUpdate()
{
	Vector2i32 p = Vector2i32::ZERO;

	if (!m_MouseState->m_IsRelative) //get mouse cursor absolute pos
		m_MouseState->m_CurButtons = SDL_GetMouseState(&p.x, &p.y);
	else //get mouse cursor relative pos
		m_MouseState->m_CurButtons = SDL_GetRelativeMouseState(&p.x, &p.y);

	m_MouseState->m_CurPos = p;
}

void Input::ProcessInput(SDL_Event event)
{
	switch (event.type)
	{
	case SDL_MOUSEWHEEL:
		m_MouseState->m_MouseScrollWheel = Vector2i32(event.wheel.x, static_cast<float>(event.wheel.y));
		break;
	default:
		break;
	}
}

const std::shared_ptr<KeyboardState> &Input::GetKeyboard()
{
	return m_KeyboardState;
}

const std::shared_ptr<MouseState> &Input::GetMouse()
{
	return m_MouseState;
}
