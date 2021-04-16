//@BridgetACasey

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <array>

enum class InputDevice
{
	RELEASED = 0,
	PRESSED,
	DOWN
};

struct Mouse
{
	InputDevice leftButton;
	InputDevice rightButton;

	float x;
	float y;
};

class InputManager
{
	friend class Window;

public:
	InputManager();
	~InputManager();

	void update();

	bool isKeyPressed(sf::Keyboard::Key key);
	bool isKeyDown(sf::Keyboard::Key key);
	bool isKeyReleased(sf::Keyboard::Key key);

private:
	void resetKeys();

	void setKeyActive(sf::Keyboard::Key key);
	void setKeyInactive(sf::Keyboard::Key key);

	void setMousePosition(float x, float y);

	Mouse mouse;

	std::array<InputDevice, sf::Keyboard::Key::KeyCount> keys = { InputDevice::RELEASED };
};