//@BridgetACasey

#pragma once

class InputManager;
class Window;

class Application
{
public:
	Application();
	~Application();

	void run();

	void render();

private:
	InputManager* inputManager;
	Window* window;
};