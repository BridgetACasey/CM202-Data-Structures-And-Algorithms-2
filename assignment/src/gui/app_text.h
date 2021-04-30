//@BridgetACasey

#include <SFML\Graphics\Text.hpp>
#include <SFML\Graphics\Font.hpp>

#pragma once

//An SFML text object with pre-assigned values for size and font
class AppText : public sf::Text
{
public:
	AppText();
	~AppText();
	
private:
	sf::Font font;
	const int defaultSize = 64;
};