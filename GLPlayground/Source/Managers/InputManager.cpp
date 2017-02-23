#include "stdafx.h"
#include "InputManager.h"

void InputManager::Initialize(GLFWwindow & Window)
{
	glfwSetKeyCallback(&Window, &InputManager::KeyCallback);
}

void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	InputManager & ManagerSingleton = GetInputManager();

	for (auto keyFunction : ManagerSingleton.KeyEvents)
	{
		keyFunction(key, KeyState::PRESSED);
	}
}

InputManager & InputManager::GetInputManager()
{
	static InputManager Manager;

	return Manager;
}
