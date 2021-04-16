//@BridgetACasey

#include "application.h"

#include "gui/input_manager.h"
#include "gui/window.h"

Application::Application()
{
	inputManager = new InputManager();
	window = new Window(inputManager);
}

Application::~Application()
{

}

void Application::run()
{
	inputManager->update();
	window->update();

	render();
}

void Application::render()
{

}