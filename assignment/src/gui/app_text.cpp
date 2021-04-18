//@BridgetACasey

#include "app_text.h"
#include "debug.h"

AppText::AppText()
{
	if (!font.loadFromFile("assets/arial.ttf"))
	{
		debugPrint("Error loading font!");
	}

	setFont(font);
	setFillColor(sf::Color::White);
	setCharacterSize(defaultSize);
	setStyle(sf::Text::Regular);
}

AppText::~AppText()
{

}