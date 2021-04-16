//@BridgetACasey

#include "input_manager.h"

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

void InputManager::update()
{
	resetKeys();
}

bool InputManager::isKeyPressed(sf::Keyboard::Key key)
{
	if (keys[key] == InputDevice::PRESSED)
	{
		return true;
	}

	return false;
}

bool InputManager::isKeyDown(sf::Keyboard::Key key)
{
	if (keys[key] == InputDevice::DOWN)
	{
		return true;
	}

	return false;
}

bool InputManager::isKeyReleased(sf::Keyboard::Key key)
{
	if (keys[key] == InputDevice::RELEASED)
	{
		return true;
	}

	return false;
}

void InputManager::resetKeys()
{
	keys.fill(InputDevice::RELEASED);
}

void InputManager::setKeyActive(sf::Keyboard::Key key)
{
	keys[key] = (keys[key] == InputDevice::PRESSED) ? InputDevice::DOWN : InputDevice::PRESSED;
}

void InputManager::setKeyInactive(sf::Keyboard::Key key)
{
	keys[key] = InputDevice::RELEASED;
}

void InputManager::setMousePosition(float x, float y)
{
	mouse.x = x;
	mouse.y = y;
}