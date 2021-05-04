//@BridgetACasey

#pragma once

class Window;
class InputManager;
class AppText;
class Button;
class TestSuite;

#include "mandelbrot.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Clock.hpp>

#include <queue>

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
	void runMandelbrotSequence();

	void writeToTGA(const char* fileName);

	bool calculatingMandelbrot;	//To check if the Mandelbrot image is currently being calculated to switch to the loading screen

	float threadCount;
	float maxItrs;

	TestSuite* testSuite;

	ImageCoordinates imageCoordinates;

	std::mutex mandelbrotMutex;
	std::queue<Mandelbrot*> mandelbrotQueue;
	std::condition_variable queueCondition;
	
	//GUI-related functions and variables
	void generateSprite();	//Converts Mandelbrot image data into RGBA pixel values to be read by SFML

	void updateCoordinates(ImageCoordinates& coords);
	
	inline float getDeltaTime() { return appClock.restart().asSeconds(); }
	
	void setupText();
	void setupButtons();
	
	float deltaTime;
	sf::Clock appClock;

	ApplicationState appState;

	InputManager* inputManager;
	Window* window;

	AppText* menuTitleText;
	AppText* loadingScreenText;
	AppText* setupTimeText;
	AppText* runTimeText;

	Button* runButton;
	Button* quitButton;
	Button* backToMenuButton;

	std::vector<Button*> arrowButtons;
	std::vector<AppText*> arrowText;

	//For creating a display from an array of pixels, SFML requires separate image, texture, and sprite objects
	sf::Image displayImage;
	sf::Texture displayTexture;
	sf::Sprite displaySprite;
};