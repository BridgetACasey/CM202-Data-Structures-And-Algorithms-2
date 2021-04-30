//@BridgetACasey

#pragma once

class InputManager;
class Window;
class AppText;
class Button;

#include "mandelbrot.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

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
	float getDeltaTime();
	
	void updateCoordinates(ImageCoordinates& coords);
	
	void setupMandelbrot();
	void runMandelbrot();

	void generateSprite();	//Converts Mandelbrot image data into RGBA pixel values to be read by SFML
	void writeToTGA(const char* fileName);
	
	float deltaTime;

	bool calculating;	//To check if the Mandelbrot image is being calculated
	int threadsUsed;

	std::queue<Mandelbrot*> mandelbrotQueue;
	std::condition_variable queueCondition;
	std::mutex mandelbrotMutex;

	ImageCoordinates imageCoordinates;
	
	ApplicationState appState;

	InputManager* inputManager;
	Window* window;

	AppText* menuTitleText;
	AppText* loadingScreenText;

	Button* runButton;
	Button* quitButton;
	Button* backButton;

	std::vector<Button*> arrowButtons;
	std::vector<AppText*> arrowText;

	sf::Clock appClock;

	//For creating a display from an array of pixels, SFML requires separate image, texture, and sprite objects
	sf::Image displayImage;
	sf::Texture displayTexture;
	sf::Sprite displaySprite;
};