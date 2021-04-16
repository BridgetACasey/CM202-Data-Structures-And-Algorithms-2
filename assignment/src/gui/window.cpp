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
    while (isOpen())
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
					inputManager->setKeyActive(event.key.code);
					break;

				case sf::Event::KeyReleased:
					inputManager->setKeyInactive(event.key.code);
					break;

				case sf::Event::MouseMoved:
					inputManager->setMousePosition(event.mouseMove.x, event.mouseMove.y);
					break;

				case sf::Event::MouseButtonPressed:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						inputManager->mouse.leftButton = (inputManager->mouse.leftButton == InputDevice::IDLE)
							? InputDevice::PRESSED : InputDevice::DOWN;
					}

					else if (event.mouseButton.button == sf::Mouse::Right)
					{
						inputManager->mouse.rightButton = (inputManager->mouse.rightButton == InputDevice::IDLE)
							? InputDevice::PRESSED : InputDevice::DOWN;
					}
					break;

				case sf::Event::MouseButtonReleased:
					if (event.mouseButton.button == sf::Mouse::Left)
					{
						inputManager->mouse.leftButton = InputDevice::RELEASED;
					}

					else if (event.mouseButton.button == sf::Mouse::Right)
					{
						inputManager->mouse.rightButton = InputDevice::RELEASED;
					}
					break;
			}
        }
    }
}

void Window::clearBuffer()
{
	clear(sf::Color(255, 255, 255, 255));
}

void Window::displayBuffer()
{
	display();
}

void Window::render(sf::Drawable& drawable)
{
	draw(drawable);
}