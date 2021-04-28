//@BridgetACasey

#include "application.h"

#include "gui/input_manager.h"
#include "gui/window.h"
#include "gui/button.h"

#include "mandelbrot.h"

#include <thread>

uint32_t image[HEIGHT][WIDTH];

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

	setupMandelbrot();
	runMandelbrot();
}

Application::~Application()
{

}

void Application::setupMandelbrot()
{
	ImageCoordinates coords;
	ImageDimensions dims;

	int totalThreads = std::thread::hardware_concurrency();

	for (int i = 0; i < totalThreads; ++i)
	{
		dims.minX = 0;
		dims.maxX = WIDTH;

		dims.minY = i * (HEIGHT / totalThreads);
		dims.maxY = dims.minY + (HEIGHT / totalThreads);

		mandelbrotMutex.lock();

			mandelbrotQueue.push(new Mandelbrot(&coords, &dims));

		mandelbrotMutex.unlock();
	}
}

void Application::runMandelbrot()
{
	int totalThreads = std::thread::hardware_concurrency();
	std::vector<std::thread*> threads;

	for (int i = 0; i < totalThreads; ++i)
	{
		threads.push_back(new std::thread([&]
		{
			if(!mandelbrotQueue.empty())
			{
				mandelbrotMutex.lock();

					Mandelbrot* current = mandelbrotQueue.front();
					mandelbrotQueue.pop();

				mandelbrotMutex.unlock();

				current->compute();

				delete current;
			}
		}));
	}

	for (int i = 0; i < totalThreads; ++i)
	{
		threads[i]->join();
	}

	generateSprite();
}

void Application::generateSprite()
{
	//Create SFML image from Mandelbrot data
	std::vector<sf::Uint8> pixels;

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			sf::Uint8 pixel[4] =
			{
				image[y][x] & 0xFF,			// blue channel
				(image[y][x] >> 8) & 0xFF,	// green channel
				(image[y][x] >> 16) & 0xFF, // red channel
				255							// alpha channel
			};

			pixels.push_back(pixel[2]);
			pixels.push_back(pixel[1]);
			pixels.push_back(pixel[0]);
			pixels.push_back(pixel[3]);
		}
	}

	displayImage.create(WIDTH, HEIGHT, pixels.data());

	displayTexture.create(WIDTH, HEIGHT);
	displayTexture.loadFromImage(displayImage, sf::IntRect());

	displaySprite.setTexture(displayTexture);
	displaySprite.setPosition(sf::Vector2f(0.0f, 0.0f));
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