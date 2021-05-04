//@BridgetACasey

#include "application.h"

#include "gui/window.h"
#include "gui/input_manager.h"
#include "gui/button.h"

#include "utility.h"
#include "test_suite.h"

const int MAX_THREADS = std::thread::hardware_concurrency();	//Max available threads relative to the system hardware

extern uint32_t image[HEIGHT][WIDTH];

Application::Application()
{
	calculatingMandelbrot = false;

	threadCount = (float)MAX_THREADS;

	maxImageItrs = (float)imageCoordinates.maxIterations;
	maxTestItrs = 5.0f;

	deltaTime = 0.0f;

	inputManager = new InputManager();
	window = new Window(inputManager);
	testSuite = new TestSuite();

	setupText();
	setupButtons();

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
	if (calculatingMandelbrot)
	{
		runMandelbrotSequence();
	}

	//Update application based on current state
	switch (appState)
	{
	case ApplicationState::MENU:
		updateCoordinates(imageCoordinates);

		if (runButton->isClicked())
		{
			calculatingMandelbrot = true;
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
		if (backToMenuButton->isClicked())
		{
			appState = ApplicationState::MENU;
		}
		break;
	}
}

void Application::render()
{
	//Render window elements based on application state
	switch (appState)
	{
	case ApplicationState::MENU:
		window->clearBuffer();

		window->render(*menuTitleText);

		window->render(*runButton);
		window->render(*runButton->getText());
		window->render(*quitButton);
		window->render(*quitButton->getText());

		for (AppText* text : optionsText)
		{
			window->render(*text);
		}

		for (Button* button : optionsButtons)
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

		for (AppText* result : resultsText)
		{
			window->render(*result);
		}

		window->render(*backToMenuButton);
		window->render(*backToMenuButton->getText());

		window->displayBuffer();
		break;
	}
}

void Application::runMandelbrotSequence()
{
	testSuite->testMandelbrot(imageCoordinates, (int)maxTestItrs, (int)threadCount, "mandelbrot-test-data.csv");

	//testSuite->testMandelbrot(imageCoordinates, 1000, 3, "mandelbrot-data-threadx3-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 4, "mandelbrot-data-threadx4-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 5, "mandelbrot-data-threadx5-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 6, "mandelbrot-data-threadx6-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 7, "mandelbrot-data-threadx7-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 8, "mandelbrot-data-threadx8-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 9, "mandelbrot-data-threadx9-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 10, "mandelbrot-data-threadx10-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 11, "mandelbrot-data-threadx11-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 12, "mandelbrot-data-threadx12-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 13, "mandelbrot-data-threadx13-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 14, "mandelbrot-data-threadx14-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 15, "mandelbrot-data-threadx15-itrx500.csv");
	//testSuite->testMandelbrot(imageCoordinates, 1000, 16, "mandelbrot-data-threadx16-itrx500.csv");
	
	calculatingMandelbrot = false;

	resultsText.at(0)->setString("TESTS RUN: " + std::to_string((int)maxTestItrs));
	resultsText.at(1)->setString("IMAGE ITERATIONS: " + std::to_string((int)maxImageItrs));
	resultsText.at(2)->setString("AVG. CPU USAGE (%): " + std::to_string(testSuite->getAverageCPU()));
	resultsText.at(3)->setString("AVG. MEMORY USED (MB): " + std::to_string(testSuite->getAverageVirtualMemory()));
	resultsText.at(4)->setString("AVG. SETUP TIME (ms): " + std::to_string(testSuite->getAverageSetupTime()));
	resultsText.at(5)->setString("AVG. CALC TIME (ms): " + std::to_string(testSuite->getAverageCalcTime()));

	//Creating the SFML sprite image and writing to the .TGA file concurrently
	std::thread* writeThread = new std::thread([&] { writeToTGA("mandelbrot-image.tga"); });
	std::thread* spriteThread = new std::thread([&] { generateSprite(); });

	writeThread->join();
	spriteThread->join();

	appState = ApplicationState::IMAGE;
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
				image[y][x] & 0xFF,			//Blue channel
				(image[y][x] >> 8) & 0xFF,	//Green channel
				(image[y][x] >> 16) & 0xFF, //Red channel
				255							//Alpha channel
			};

			pixels.push_back(pixel[2]);	//Red
			pixels.push_back(pixel[1]);	//Green
			pixels.push_back(pixel[0]);	//Blue
			pixels.push_back(pixel[3]);	//Alpha
		}
	}

	displayImage.create(WIDTH, HEIGHT, pixels.data());

	displayTexture.create(WIDTH, HEIGHT);
	displayTexture.loadFromImage(displayImage, sf::IntRect());

	displaySprite.setTexture(displayTexture);
	displaySprite.setPosition(sf::Vector2f(0.0f, 0.0f));
}

void Application::updateCoordinates(ImageCoordinates& coords)
{
	//Clamping all the mandelbrot settings between their max and min values
	clamp(maxTestItrs, 1.0f, 1000.0f);
	clamp(threadCount, 3.0f, MAX_THREADS);
	clamp(maxImageItrs, 100.0f, 1000.0f);
	clamp(coords.top, 0.0, 1.125);
	clamp(coords.bottom, -1.125, 0.0);
	clamp(coords.left, -2.0, 0.0);
	clamp(coords.right, 0.0, 1.0);

	//Setting the updated values to their corresponding text objects
	optionsText.at(0)->setString("Test Itrs:       " + std::to_string((int)maxTestItrs));
	optionsText.at(1)->setString("Threads:       " + std::to_string((int)threadCount));
	optionsText.at(2)->setString("Image Itrs:    " + std::to_string((int)maxImageItrs));
	optionsText.at(3)->setString("Top:         " + std::to_string(coords.top));
	optionsText.at(4)->setString("Bottom:  " + std::to_string(coords.bottom));
	optionsText.at(5)->setString("Left:        " + std::to_string(coords.left));
	optionsText.at(6)->setString("Right:      " + std::to_string(coords.right));

	//Incrementing and decrementing values based on which arrow buttons have been clicked
	//Maximum test iterations
	if (optionsButtons.at(0)->isClicked())
		adjustValue(maxTestItrs, false, 25.0f, deltaTime);
	else if (optionsButtons.at(7)->isClicked())
		adjustValue(maxTestItrs, true, 25.0f, deltaTime);

	//Threads used
	if (optionsButtons.at(1)->isClicked())
		adjustValue(threadCount, false, 5.0f, deltaTime);
	else if (optionsButtons.at(8)->isClicked())
		adjustValue(threadCount, true, 5.0f, deltaTime);

	//Maximum image iterations
	if (optionsButtons.at(2)->isClicked())
		adjustValue(maxImageItrs, false, 75.0f, deltaTime);
	else if (optionsButtons.at(9)->isClicked())
		adjustValue(maxImageItrs, true, 75.0f, deltaTime);
	
	//Image coordinates - top
	if (optionsButtons.at(3)->isClicked())
		adjustValue(coords.top, false, 0.5, deltaTime);
	else if (optionsButtons.at(10)->isClicked())
		adjustValue(coords.top, true, 0.5, deltaTime);

	//Image coordinates - bottom
	if (optionsButtons.at(4)->isClicked())
		adjustValue(coords.bottom, false, 0.5, deltaTime);
	else if (optionsButtons.at(11)->isClicked())
		adjustValue(coords.bottom, true, 0.5, deltaTime);

	//Image coordinates - left
	if (optionsButtons.at(5)->isClicked())
		adjustValue(coords.left, false, 0.5, deltaTime);
	else if (optionsButtons.at(12)->isClicked())
		adjustValue(coords.left, true, 0.5, deltaTime);

	//Image coordinates - right
	if (optionsButtons.at(6)->isClicked())
		adjustValue(coords.right, false, 0.5, deltaTime);
	else if (optionsButtons.at(13)->isClicked())
		adjustValue(coords.right, true, 0.5, deltaTime);
}

void Application::setupText()
{
	//Creating text objects
	menuTitleText = new AppText();
	menuTitleText->setPosition(sf::Vector2f(190.0f, 50.0f));
	menuTitleText->setCharacterSize(72);
	menuTitleText->setString("INTERACTIVE MANDELBROT SET");

	loadingScreenText = new AppText();
	loadingScreenText->setPosition(sf::Vector2f(550.0f, 360.0f));
	loadingScreenText->setCharacterSize(72);
	loadingScreenText->setString("CALCULATING");

	//Image screen results text
	AppText* testItrsText = new AppText();
	testItrsText->setPosition(sf::Vector2f(40.0f, 580.0f));
	testItrsText->setCharacterSize(36);
	resultsText.push_back(testItrsText);

	AppText* imageItrsText = new AppText();
	imageItrsText->setPosition(sf::Vector2f(40.0f, 630.0f));
	imageItrsText->setCharacterSize(36);
	resultsText.push_back(imageItrsText);

	AppText* cpuUsageText = new AppText();
	cpuUsageText->setPosition(sf::Vector2f(40.0f, 680.0f));
	cpuUsageText->setCharacterSize(36);
	resultsText.push_back(cpuUsageText);

	AppText* memoryUsageText = new AppText();
	memoryUsageText->setPosition(sf::Vector2f(40.0f, 730.0f));
	memoryUsageText->setCharacterSize(36);
	resultsText.push_back(memoryUsageText);

	AppText* setupTimeText = new AppText();
	setupTimeText->setPosition(sf::Vector2f(40.0f, 780.0f));
	setupTimeText->setCharacterSize(36);
	resultsText.push_back(setupTimeText);

	AppText* calcTimeText = new AppText();
	calcTimeText->setPosition(sf::Vector2f(40.0f, 830.0f));
	calcTimeText->setCharacterSize(36);
	resultsText.push_back(calcTimeText);

	//Menu button text
	AppText* testItr = new AppText();
	testItr->setPosition(sf::Vector2f(365.0f, 190.0f));
	testItr->setString(std::to_string((int)maxTestItrs));
	optionsText.push_back(testItr);

	AppText* threads = new AppText();
	threads->setPosition(sf::Vector2f(365.0f, 290.0f));
	threads->setString(std::to_string((int)threadCount));
	optionsText.push_back(threads);

	AppText* imageItr = new AppText();
	imageItr->setPosition(sf::Vector2f(365.0f, 390.0f));
	imageItr->setString(std::to_string(imageCoordinates.maxIterations));
	optionsText.push_back(imageItr);

	AppText* top = new AppText();
	top->setPosition(sf::Vector2f(365.0f, 490.0f));
	top->setString(std::to_string(imageCoordinates.top));
	optionsText.push_back(top);

	AppText* bottom = new AppText();
	bottom->setPosition(sf::Vector2f(365.0f, 590.0f));
	bottom->setString(std::to_string(imageCoordinates.bottom));
	optionsText.push_back(bottom);

	AppText* left = new AppText();
	left->setPosition(sf::Vector2f(365.0f, 690.0f));
	left->setString(std::to_string(imageCoordinates.left));
	optionsText.push_back(left);

	AppText* right = new AppText();
	right->setPosition(sf::Vector2f(365.0f, 790.0f));
	right->setString(std::to_string(imageCoordinates.right));
	optionsText.push_back(right);
}

void Application::setupButtons()
{
	//Creating button objects
	runButton = new Button(inputManager);
	runButton->setPosition(sf::Vector2f(1150.0f, 290.0f));
	runButton->setSize(sf::Vector2f(250.0f, 75.0f));
	runButton->setText("    RUN");

	quitButton = new Button(inputManager);
	quitButton->setPosition(sf::Vector2f(1150.0f, 550.0f));
	quitButton->setSize(sf::Vector2f(250.0f, 75.0f));
	quitButton->setText("    QUIT");

	backToMenuButton = new Button(inputManager);
	backToMenuButton->setPosition(sf::Vector2f(1365.0f, 800.0f));
	backToMenuButton->setSize(sf::Vector2f(175.0f, 60.0f));
	backToMenuButton->setText(" MENU");

	for (int i = 0; i < 7; ++i)
	{
		Button* lowerArrow = new Button(inputManager);
		lowerArrow->setPosition(sf::Vector2f(200.0f, 190.0f + (i * 100.0f)));
		lowerArrow->setSize(sf::Vector2f(50.0f, 50.0f));
		lowerArrow->setText(" <");

		optionsButtons.push_back(lowerArrow);
	}

	for (int i = 0; i < 7; ++i)
	{
		Button* upperArrow = new Button(inputManager);
		upperArrow->setPosition(sf::Vector2f(900.0f, 190.0f + (i * 100.0f)));
		upperArrow->setSize(sf::Vector2f(50.0f, 50.0f));
		upperArrow->setText(" >");

		optionsButtons.push_back(upperArrow);
	}
}