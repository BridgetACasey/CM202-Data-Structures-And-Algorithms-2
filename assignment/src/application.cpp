//@BridgetACasey

#include "application.h"

#include "gui/input_manager.h"
#include "gui/window.h"
#include "debug.h"

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
	while (window->isOpen())
	{
		//inputManager->update();
		window->update();

		if (inputManager->isKeyPressed(sf::Keyboard::Key::R))
		{
			debugPrint("R IS PRESSED\n");
		}

		else if (inputManager->isKeyDown(sf::Keyboard::Key::R))
		{
			debugPrint("R IS DOWN\n");
		}

		else if (inputManager->isKeyReleased(sf::Keyboard::Key::R))
		{
			debugPrint("R IS RELEASED\n");
		}

		render();
	}
}

void Application::render()
{

}