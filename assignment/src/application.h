//@BridgetACasey

#pragma once

class InputManager;
class Window;
class AppText;
class Button;
class Mandelbrot;

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <queue>
#include <mutex>

class Application
{
public:
	Application();
	~Application();

	void run();

	void render();

private:
	void setupMandelbrot();
	void runMandelbrot();

	void generateSprite();

	InputManager* inputManager;
	Window* window;

	AppText* title;
	Button* button;

	std::queue<Mandelbrot*> mandelbrotQueue;

	std::mutex mandelbrotMutex;

	sf::Image displayImage;
	sf::Texture displayTexture;
	sf::Sprite displaySprite;
};