//@BridgetACasey

#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

struct Mouse
{
	bool left = false;
	bool right = false;

	float x;
	float y;
};

class InputManager
{
	friend class Window;

public:
	InputManager();
	~InputManager();

	bool isKeyPressed(sf::Keyboard::Key key) const;
	bool isKeyReleased(sf::Keyboard::Key key) const;

	inline Mouse& getMouse() { return mouse; }

private:
	inline void setKeyPressed(sf::Keyboard::Key key) { keys[key] = true; }
	inline void setKeyReleased(sf::Keyboard::Key key) { keys[key] = false; }

	void setMousePosition(float x, float y);

	Mouse mouse;

	bool keys[sf::Keyboard::Key::KeyCount] = { 0 };
};