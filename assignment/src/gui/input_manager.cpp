//@BridgetACasey

#include "input_manager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

bool InputManager::isKeyPressed(sf::Keyboard::Key key) const
{
	if (keys[key])
	{
		return true;
	}

	return false;
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key) const
{
	if (keys[key])
	{
		return false;
	}

	return true;
}

void InputManager::setMousePosition(float x, float y)
{
	mouse.x = x;
	mouse.y = y;
}