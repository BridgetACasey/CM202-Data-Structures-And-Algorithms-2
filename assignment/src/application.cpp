//@BridgetACasey

#include "application.h"

#include "gui/input_manager.h"
#include "gui/window.h"
#include "gui/button.h"

#include "mandelbrot.h"

Application::Application()
{
	inputManager = new InputManager();
	window = new Window(inputManager);

	title = new AppText();
	title->setPosition(sf::Vector2f(575.0f, 100.0f));
	title->setString("MANDELBROT");

	button = new Button(inputManager);
	button->setPosition(sf::Vector2f(300.0f, 300.0f));
	button->setSize(sf::Vector2f(150.0f, 50.0f));
	button->setText("TEXT");

	mandelbrot = new Mandelbrot();

	mandelbrot->compute(-2.0, 1.0, 1.125, -1.125);
	mandelbrot->assignPixels();
	//mandelbrot->writeToTGA("output/output.tga");

	displayTexture.create(1600, 900);
	displayTexture.loadFromImage(mandelbrot->getImage(), sf::IntRect());

	displaySprite.setTexture(displayTexture);
	displaySprite.setPosition(sf::Vector2f(0.0f, 0.0f));
}

Application::~Application()
{

}

void Application::run()
{
	while (window->isOpen())
	{
		window->update();

		render();
	}
}

void Application::render()
{
	window->clearBuffer();

	//Render window elements
	//window->render(*title);

	//window->render(*button);
	//window->render(*button->getText());

	window->render(displaySprite);

	window->displayBuffer();
}