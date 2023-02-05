#pragma once
#include "InputListener.h"
#include <unordered_set>
#include "Point.h"

class InputSystem
{
private:
	InputSystem();
	~InputSystem();
public:
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);
	void update();
	void setInputPosition(const Point& posision);
	void showCursor(bool show);

public:
	static InputSystem* get();
	static void create();
	static void release();
private:
	std::unordered_set<InputListener*> m_set_listeners;
	unsigned char m_keys_states[256] = {};
	unsigned char m_old_keys_states[256] = {};
	Point m_old_mouse_position;
	bool m_first_time = true;
	static InputSystem* m_input;


};

