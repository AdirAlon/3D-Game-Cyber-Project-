#include "InputSystem.h"
#include "Window.h"
#include "Point.h"

InputSystem* InputSystem::m_input = nullptr;

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
	InputSystem::m_input = nullptr;
}

void InputSystem::addListener(InputListener* listener)
{
	m_set_listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	m_set_listeners.erase(listener);
}

void InputSystem::update()
{
	POINT mouse_position_current = {};
	::GetCursorPos(&mouse_position_current);

	if (m_first_time)
	{
		m_old_mouse_position = Point(mouse_position_current.x, mouse_position_current.y);
		m_first_time = false;
	}

	if (mouse_position_current.x != m_old_mouse_position.m_x || mouse_position_current.y != m_old_mouse_position.m_y)
	{
		std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

		while (it != m_set_listeners.end())
		{
			(*it)->onMouseMove(Point(mouse_position_current.x, mouse_position_current.y));
			++it;
		}
	}

	m_old_mouse_position = Point(mouse_position_current.x, mouse_position_current.y);

	if (::GetKeyboardState((m_keys_states)))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			//Down
			if (m_keys_states[i] & 0x80)
			{
				std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

				while (it != m_set_listeners.end())
				{
					if (i == VK_LBUTTON)
					{
						if (m_keys_states[i] != m_old_keys_states[i])
							(*it)->onLeftMouseDown(Point(mouse_position_current.x, mouse_position_current.y));
					}

					if (i == VK_RBUTTON)
					{
						if (m_keys_states[i] != m_old_keys_states[i])
							(*it)->onRightMouseDown(Point(mouse_position_current.x, mouse_position_current.y));
					}
					else
						(*it)->onKeyDown(i);
					++it;
				}
					
			}
			//Up
			else
			{
				if (m_keys_states[i] != m_old_keys_states[i])
				{
					std::unordered_set<InputListener*>::iterator it = m_set_listeners.begin();

					while (it != m_set_listeners.end())
					{
						if (i == VK_LBUTTON)
							(*it)->onLeftMouseUp(Point(mouse_position_current.x, mouse_position_current.y));
						else if (i == VK_RBUTTON)
							(*it)->onRightMouseUp(Point(mouse_position_current.x, mouse_position_current.y));
						else
							(*it)->onKeyUp(i);
						++it;
					}
				}
			}
		}
		::memcpy(m_old_keys_states, m_keys_states, sizeof(unsigned char) * 256);
	}
}

void InputSystem::setInputPosition(const Point& posision)
{
	::SetCursorPos(posision.m_x, posision.m_y);
}

void InputSystem::showCursor(bool show)
{
	::ShowCursor(show);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}

void InputSystem::create()
{
	if (InputSystem::m_input)
		throw std::exception("InputSystem Exist");
	InputSystem::m_input = new InputSystem();
}

void InputSystem::release()
{
	if (!InputSystem::m_input)
		return;
	delete InputSystem::m_input;
}
