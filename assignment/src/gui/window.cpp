//@BridgetACasey

#include "window.h"

#include <SFML/Window/Event.hpp>

#include "input_manager.h"

Window::Window(InputManager* input)
{
	inputManager = input;

	create(sf::VideoMode(1600, 900), "Application", sf::Style::Titlebar | sf::Style::Close);
}

Window::~Window()
{

}

void Window::update()
{
    sf::Event event;

    while (pollEvent(event))
    {
		switch (event.type)
		{
			case sf::Event::Closed:
				close();
				break;

			case sf::Event::KeyPressed:
				inputManager->setKeyPressed(event.key.code);
				break;

			case sf::Event::KeyReleased:
				inputManager->setKeyReleased(event.key.code);
				break;

			case sf::Event::MouseMoved:
				inputManager->setMousePosition(event.mouseMove.x, event.mouseMove.y);
				break;

			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					inputManager->getMouse().left = true;
				}

				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					inputManager->getMouse().right = true;
				}
				break;

			case sf::Event::MouseButtonReleased:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					inputManager->getMouse().left = false;
				}

				else if (event.mouseButton.button == sf::Mouse::Right)
				{
					inputManager->getMouse().right = false;
				}
				break;

			default:
				break;
		}
    }
}

void Window::clearBuffer()
{
	clear(sf::Color(0, 0, 0, 255));
}

void Window::displayBuffer()
{
	display();
}

void Window::render(sf::Drawable& drawable)
{
	draw(drawable);
}