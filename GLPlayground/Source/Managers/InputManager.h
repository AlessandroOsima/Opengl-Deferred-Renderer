#pragma once
#include "Input/Input.h"
#include <list>

using KeyCallbackFunction = std::function<void(KeyState, KeyCode)>;

class InputManager
{
public:
	void Initialize(GLFWwindow & Window);

	static InputManager & GetInputManager();

	void operator+=(KeyCallbackFunction Function);

private:
	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	std::list<KeyCallbackFunction> KeyEvents;
};

