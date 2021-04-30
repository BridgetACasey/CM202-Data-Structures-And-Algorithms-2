//@BridgetACasey

#include "application.h"

#include "gui/input_manager.h"
#include "gui/window.h"
#include "gui/button.h"

#include "mandelbrot.h"

#include <thread>

const int MAX_THREADS = std::thread::hardware_concurrency();	//Max available threads relative to the system hardware

extern uint32_t image[HEIGHT][WIDTH];

Application::Application()
{
	calculating = false;

	inputManager = new InputManager();
	window = new Window(inputManager);

	//Creating text objects
	title = new AppText();
	title->setPosition(sf::Vector2f(500.0f, 100.0f));
	title->setString("MANDELBROT SET");

	loading = new AppText();
	loading->setPosition(sf::Vector2f(550.0f, 375.0f));
	loading->setString("CALCULATING");

	//Creating button objects
	runButton = new Button(inputManager);
	runButton->setPosition(sf::Vector2f(1200.0f, 400.0f));
	runButton->setSize(sf::Vector2f(150.0f, 50.0f));
	runButton->setText("RUN");

	quitButton = new Button(inputManager);
	quitButton->setPosition(sf::Vector2f(1200.0f, 600.0f));
	quitButton->setSize(sf::Vector2f(150.0f, 50.0f));
	quitButton->setText("QUIT");

	backButton = new Button(inputManager);
	backButton->setPosition(sf::Vector2f(100.0f, 200.0f));
	backButton->setSize(sf::Vector2f(150.0f, 50.0f));
	backButton->setText("BACK");

	appState = ApplicationState::MENU;
}

Application::~Application()
{

}

void Application::run()
{
	while (window->isOpen())
	{
		window->update();

		update();

		render();
	}
}

void Application::update()
{
	//Checking for this before the switch statement so the loading screen is displayed before calculations begin
	if (calculating)
	{
		runMandelbrot();
	}

	switch (appState)
	{
		case ApplicationState::MENU:
			if (runButton->isClicked())
			{
				calculating = true;
				appState = ApplicationState::LOADING;
			}

			if (quitButton->isClicked())
			{
				window->close();
			}
			break;

		case ApplicationState::LOADING:
			break;

		case ApplicationState::IMAGE:
			if (backButton->isClicked())
			{
				appState = ApplicationState::MENU;
			}
			break;
	}
}

void Application::render()
{
	//Render window elements
	switch (appState)
	{
		case ApplicationState::MENU:
			window->clearBuffer();

			window->render(*title);

			window->render(*runButton);
			window->render(*runButton->getText());
			window->render(*quitButton);
			window->render(*quitButton->getText());

			window->displayBuffer();
			break;

		case ApplicationState::LOADING:
			window->clearBuffer();

			window->render(*loading);

			window->displayBuffer();
			break;

		case ApplicationState::IMAGE:
			window->clearBuffer();

			window->render(displaySprite);

			window->render(*backButton);
			window->render(*backButton->getText());

			window->displayBuffer();
			break;
	}
}

void Application::setupMandelbrot()
{
	ImageCoordinates coords;
	ImageDimensions dims;

	//Setting the parameters for each image strip relative to the number of assigned threads
	for (int i = 0; i < MAX_THREADS; ++i)
	{
		dims.minX = 0;
		dims.maxX = WIDTH;

		dims.minY = i * (HEIGHT / MAX_THREADS);
		dims.maxY = dims.minY + (HEIGHT / MAX_THREADS);

		mandelbrotQueue.push(new Mandelbrot(&coords, &dims));
	}
}

void Application::runMandelbrot()
{
	setupMandelbrot();

	std::vector<std::thread*> threads;

	for (int i = 0; i < MAX_THREADS; ++i)
	{
		threads.push_back(new std::thread([&]
		{
			std::unique_lock<std::mutex> lock(mandelbrotMutex);
			queueCondition.wait(lock, [this]() { return !mandelbrotQueue.empty(); });

				Mandelbrot* current = mandelbrotQueue.front();
				mandelbrotQueue.pop();

			lock.unlock();
			queueCondition.notify_all();

			current->compute();

			delete current;
		}));
	}

	for (int i = 0; i < MAX_THREADS; ++i)
	{
		threads[i]->join();
	}

	calculating = false;

	//Creating the SFML sprite image and writing to the .TGA file concurrently
	std::thread* spriteThread = new std::thread([&] { generateSprite(); });
	std::thread* writeThread = new std::thread([&] { writeToTGA("output/output.tga"); });

	spriteThread->join();
	writeThread->join();

	appState = ApplicationState::IMAGE;
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

void Application::writeToTGA(const char* fileName)
{
	std::ofstream outfile(fileName, std::ofstream::binary);

	uint8_t header[18] =
	{
		0, // no image ID
		0, // no colour map
		2, // uncompressed 24-bit image
		0, 0, 0, 0, 0, // empty colour map specification
		0, 0, // X origin
		0, 0, // Y origin
		WIDTH & 0xFF, (WIDTH >> 8) & 0xFF, // width
		HEIGHT & 0xFF, (HEIGHT >> 8) & 0xFF, // height
		24, // bits per pixel
		0, // image descriptor
	};

	outfile.write((const char*)header, 18);

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			uint8_t pixel[3] =
			{
				image[y][x] & 0xFF, // blue channel
				(image[y][x] >> 8) & 0xFF, // green channel
				(image[y][x] >> 16) & 0xFF, // red channel
			};

			outfile.write((const char*)pixel, 3);
		}
	}

	outfile.close();

	if (!outfile)
	{
		// An error has occurred at some point since we opened the file.
		//cout << "Error writing to " << filename << endl;
		exit(1);
	}
}