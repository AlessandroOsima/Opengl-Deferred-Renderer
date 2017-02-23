#pragma once
#include <GLFW/glfw3.h>
#include <list>

enum class  KeyState
{
	PRESSED,
	RELEASED
};

class InputManager
{
public:
	void Initialize(GLFWwindow & Window);

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static InputManager & GetInputManager();

private:
	std::list<std::function<void(int, KeyState)>> KeyEvents;
};

