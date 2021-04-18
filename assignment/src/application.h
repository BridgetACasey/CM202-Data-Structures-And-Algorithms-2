//@BridgetACasey

#pragma once

class InputManager;
class Window;
class AppText;
class Button;

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

	AppText* title;

	Button* button;
};