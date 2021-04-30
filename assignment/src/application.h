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

//Allows for easy switching between states in the Mandelbrot GUI
enum class ApplicationState
{
	MENU = 0,
	LOADING,
	IMAGE
};

class Application
{
public:
	Application();
	~Application();

	void run();

	void update();	//Update SFML-related functions
	void render();	//Render sprites to screen

private:
	void setupMandelbrot();
	void runMandelbrot();

	void generateSprite();	//Converts Mandelbrot image data into RGBA pixel values to be read by SFML
	void writeToTGA(const char* fileName);

	bool calculating;	//To check if the Mandelbrot image is being calculated
	
	ApplicationState appState;

	InputManager* inputManager;
	Window* window;

	AppText* title;
	AppText* loading;

	Button* runButton;
	Button* quitButton;
	Button* backButton;

	std::queue<Mandelbrot*> mandelbrotQueue;
	std::condition_variable queueCondition;
	std::mutex mandelbrotMutex;

	//For creating a display from an array of pixels, SFML requires separate image, texture, and sprite objects
	sf::Image displayImage;
	sf::Texture displayTexture;
	sf::Sprite displaySprite;
};