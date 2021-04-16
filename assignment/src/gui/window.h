//@BridgetACasey

#include <SFML/Graphics/RenderWindow.hpp>

class InputManager;

class Window : public sf::RenderWindow
{
public:
	Window(InputManager* input);
	~Window();

	void update();

	void clearBuffer();
	void displayBuffer();

	void render(sf::Drawable& drawable);

private:
	InputManager* inputManager;
};