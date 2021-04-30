//@BridgetACasey

#include "application.h"

#include "gui/input_manager.h"
#include "gui/window.h"
#include "gui/button.h"

#include "utility.h"

#include <thread>

const int MAX_THREADS = std::thread::hardware_concurrency();	//Max available threads relative to the system hardware

extern uint32_t image[HEIGHT][WIDTH];

Application::Application()
{
	calculating = false;
	threadsUsed = MAX_THREADS;

	deltaTime = 0.0f;

	inputManager = new InputManager();
	window = new Window(inputManager);

	//Creating text objects
	menuTitleText = new AppText();
	menuTitleText->setPosition(sf::Vector2f(500.0f, 100.0f));
	menuTitleText->setCharacterSize(64);
	menuTitleText->setString("MANDELBROT SET");

	loadingScreenText = new AppText();
	loadingScreenText->setPosition(sf::Vector2f(550.0f, 375.0f));
	loadingScreenText->setCharacterSize(64);
	loadingScreenText->setString("CALCULATING");

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

	for (int i = 0; i < 6; ++i)
	{
		Button* lowerArrow = new Button(inputManager);
		lowerArrow->setPosition(sf::Vector2f(100.0f, 250.0f + (i * 100.0f)));
		lowerArrow->setSize(sf::Vector2f(50.0f, 50.0f));
		lowerArrow->setText("<");

		arrowButtons.push_back(lowerArrow);
	}

	for (int i = 0; i < 6; ++i)
	{
		Button* upperArrow = new Button(inputManager);
		upperArrow->setPosition(sf::Vector2f(650.0f, 250.0f + (i * 100.0f)));
		upperArrow->setSize(sf::Vector2f(50.0f, 50.0f));
		upperArrow->setText(">");

		arrowButtons.push_back(upperArrow);
	}

	AppText* top = new AppText();
	top->setPosition(sf::Vector2f(150.0f, 250.0f));
	top->setString(std::to_string(imageCoordinates.top));
	arrowText.push_back(top);

	AppText* bottom = new AppText();
	bottom->setPosition(sf::Vector2f(150.0f, 350.0f));
	bottom->setString(std::to_string(imageCoordinates.bottom));
	arrowText.push_back(bottom);

	AppText* left = new AppText();
	left->setPosition(sf::Vector2f(150.0f, 450.0f));
	left->setString(std::to_string(imageCoordinates.left));
	arrowText.push_back(left);

	AppText* right = new AppText();
	right->setPosition(sf::Vector2f(150.0f, 550.0f));
	right->setString(std::to_string(imageCoordinates.right));
	arrowText.push_back(right);

	AppText* maxItr = new AppText();
	maxItr->setPosition(sf::Vector2f(150.0f, 650.0f));
	maxItr->setString(std::to_string(imageCoordinates.maxIterations));
	arrowText.push_back(maxItr);

	AppText* threads = new AppText();
	threads->setPosition(sf::Vector2f(150.0f, 750.0f));
	threads->setString(std::to_string(threadsUsed));
	arrowText.push_back(threads);

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
	deltaTime = getDeltaTime();

	//Checking for this before the switch statement so the loading screen is displayed before calculations begin
	if (calculating)
	{
		runMandelbrot();
	}

	switch (appState)
	{
		case ApplicationState::MENU:
			updateCoordinates(imageCoordinates);
			
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

			window->render(*menuTitleText);

			window->render(*runButton);
			window->render(*runButton->getText());
			window->render(*quitButton);
			window->render(*quitButton->getText());

			for (AppText* text : arrowText)
			{
				window->render(*text);
			}

			for (Button* button : arrowButtons)
			{
				window->render(*button);
				window->render(*button->getText());
			}

			window->displayBuffer();
			break;

		case ApplicationState::LOADING:
			window->clearBuffer();

			window->render(*loadingScreenText);

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

float Application::getDeltaTime()
{
	return appClock.restart().asSeconds();
}

void Application::updateCoordinates(ImageCoordinates& coords)
{
	clamp(coords.top, 0.0, 1.125);
	clamp(coords.bottom, -1.125, 0.0);
	clamp(coords.left, -2.0, 0.0);
	clamp(coords.right, 0.0, 1.0);
	clamp(coords.maxIterations, 100.0, 1000.0);
	clamp(threadsUsed, 3, MAX_THREADS);

	arrowText.at(0)->setString("  Top:   " + std::to_string(imageCoordinates.top));
	arrowText.at(1)->setString("  Bottom:   " + std::to_string(imageCoordinates.bottom));
	arrowText.at(2)->setString("  Left:   " + std::to_string(imageCoordinates.left));
	arrowText.at(3)->setString("  Right:   " + std::to_string(imageCoordinates.right));
	arrowText.at(4)->setString("  Max Itr:   " + std::to_string(imageCoordinates.maxIterations));
	arrowText.at(5)->setString("  Threads:   " + std::to_string(threadsUsed));

	if (arrowButtons.at(0)->isClicked())
		adjustValue(coords.top, false, deltaTime);

	else if (arrowButtons.at(1)->isClicked())
		adjustValue(coords.bottom, false, deltaTime);

	else if (arrowButtons.at(2)->isClicked())
		adjustValue(coords.left, false, deltaTime);

	else if (arrowButtons.at(3)->isClicked())
		adjustValue(coords.right, false, deltaTime);

	else if (arrowButtons.at(4)->isClicked())
		adjustValue(coords.maxIterations, false, deltaTime);

	else if (arrowButtons.at(5)->isClicked())
		adjustValue(threadsUsed, false, deltaTime);

	else if (arrowButtons.at(6)->isClicked())
		adjustValue(coords.top, true, deltaTime);

	else if (arrowButtons.at(7)->isClicked())
		adjustValue(coords.bottom, true, deltaTime);

	else if (arrowButtons.at(8)->isClicked())
		adjustValue(coords.left, true, deltaTime);

	else if (arrowButtons.at(9)->isClicked())
		adjustValue(coords.right, true, deltaTime);

	else if (arrowButtons.at(10)->isClicked())
		adjustValue(coords.maxIterations, true, deltaTime);

	else if (arrowButtons.at(11)->isClicked())
		adjustValue(threadsUsed, true, deltaTime);
}

void Application::setupMandelbrot()
{
	ImageDimensions dims;

	//Setting the parameters for each image strip relative to the number of assigned threads
	for (int i = 0; i < threadsUsed; ++i)
	{
		dims.minX = 0;
		dims.maxX = WIDTH;

		dims.minY = i * (HEIGHT / threadsUsed);
		dims.maxY = dims.minY + (HEIGHT / threadsUsed);

		mandelbrotQueue.push(new Mandelbrot(&imageCoordinates, &dims));
	}
}

void Application::runMandelbrot()
{
	setupMandelbrot();

	std::vector<std::thread*> threads;

	for (int i = 0; i < threadsUsed; ++i)
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

	for (int i = 0; i < threadsUsed; ++i)
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