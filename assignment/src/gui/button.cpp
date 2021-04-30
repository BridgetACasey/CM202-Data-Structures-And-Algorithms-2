//@BridgetACasey

#include "button.h"

#include "input_manager.h"

Button::Button(InputManager* input)
{
	inputManager = input;
	text = new AppText();
}

Button::~Button()
{
}

void Button::setText(const char* txt)	//Sets a text object to appear inside the button boundaries
{
	text->setPosition(sf::Vector2f(getPosition().x, getPosition().y));
	text->setCharacterSize(getSize().y * 0.75f);
	text->setFillColor(sf::Color::Black);
	text->setString(txt);
}

bool Button::isHovering()	//Returns true and changes the object colour if the mouse is overlapping its position
{
	if (inputManager->getMouse().x > getPosition().x && inputManager->getMouse().x < getPosition().x + getSize().x)
	{
		if (inputManager->getMouse().y > getPosition().y && inputManager->getMouse().y < getPosition().y + getSize().y)
		{
			setFillColor(sf::Color::Red);
			return true;
		}
	}

	setFillColor(sf::Color::White);
	return false;
}

bool Button::isClicked()
{
	if (isHovering())
	{
		if (inputManager->getMouse().left)
		{
			return true;
		}
	}

	return false;
}