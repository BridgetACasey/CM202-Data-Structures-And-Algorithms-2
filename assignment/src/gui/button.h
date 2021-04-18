//@BridgetACasey

#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include "app_text.h"

class InputManager;

class Button : public sf::RectangleShape
{
public:
	Button(InputManager* input);
	~Button();

	void setText(const char* txt);

	bool isHovering();
	bool isClicked();

	inline AppText* getText() { return text; }

private:
	InputManager* inputManager;
	AppText* text;
};