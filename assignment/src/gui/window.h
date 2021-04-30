//@BridgetACasey

#include <SFML/Graphics/RenderWindow.hpp>

class InputManager;

//The SFML render window, responsible for managing window and user input events
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