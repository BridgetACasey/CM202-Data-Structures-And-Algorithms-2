//@BridgetACasey

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

enum class InputDevice
{
	IDLE = 0,
	PRESSED,
	DOWN,
	RELEASED
};

struct Mouse
{
	InputDevice leftButton;
	InputDevice rightButton;

	float x;
	float y;
};

namespace sf
{
	enum Key;
}

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

	InputDevice keys[sf::Keyboard::Key::KeyCount] = { InputDevice::IDLE };
};