#include <Windows.h>
#include <thread>
#include <chrono>
#include <vector>
#include "Game.h"
#include "GameObject.h"

const static std::chrono::milliseconds MS_PER_TICK(16); // 60 TPS

static unsigned int mapWidth, mapHeight, mapArea;
static std::vector<CR::GameObject*> gameObjects;

static HANDLE console;
static wchar_t* conScreen = new wchar_t[mapArea];
static WORD* conAttributes = new WORD[mapArea];
static DWORD bytesWrittenDump = 0; // necessary to provide for a Windows Console API function call
static bool flip = true;

namespace CR::Engine {
	static void render() {
		// Clear the screen and attributes arrays
		std::fill_n(conScreen, mapArea, ' ');
		std::fill_n(conAttributes, mapArea, 0);

		// Place every GameObject on the map
		for (auto object : gameObjects) {
			int index = object->getY() * mapWidth + object->getX();
			
			conScreen[index] = object->getSkin();
			conAttributes[index] = object->getSkinColor();

			if (flip)
				conAttributes[index] |= COMMON_LVB_REVERSE_VIDEO;
		}
		
		flip = !flip;

		// Write the screen array to the console's screen buffer
		WriteConsoleOutputCharacter(console, conScreen, mapArea, { 0,0 }, &bytesWrittenDump);
		WriteConsoleOutputAttribute(console, conAttributes, mapArea, { 0,0 }, &bytesWrittenDump);
	}

	void start() {
		// Set up the console
		console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(console);
		SetConsoleTitleA("Roguelike");
		COORD* c = new COORD();
		SetConsoleDisplayMode(console, CONSOLE_FULLSCREEN, c);
		delete c;

		COORD largestWindowSize = GetLargestConsoleWindowSize(console);

		mapWidth = largestWindowSize.X;
		mapHeight = largestWindowSize.Y;
		mapArea = mapWidth * mapHeight;
		delete[] conScreen;
		conScreen = new wchar_t[mapArea];
		delete[] conAttributes;
		conAttributes = new WORD[mapArea];

		SetConsoleScreenBufferSize(console, { (short)mapWidth, (short)mapHeight });

		//CONSOLE_SCREEN_BUFFER_INFO* sbi = new CONSOLE_SCREEN_BUFFER_INFO();
		//GetConsoleScreenBufferInfo(console, sbi);

		CONSOLE_CURSOR_INFO* ci = new CONSOLE_CURSOR_INFO();
		ci->dwSize = 1;
		ci->bVisible = FALSE;

		SetConsoleCursorInfo(console, ci);
		
		using hrclock = std::chrono::high_resolution_clock;

		std::chrono::nanoseconds lag(0);
		auto last = hrclock::now();
		
		while (true) {
			auto current = hrclock::now();
			auto elapsed = current - last;
			last = current;
			lag += elapsed;

			// Input processing

			while (lag >= MS_PER_TICK) {
				lag -= MS_PER_TICK;

				// game logic
			}

			render();

			std::this_thread::sleep_for(std::chrono::milliseconds(350));
		}
	}

	void addGameObject(CR::GameObject* object) {
		gameObjects.push_back(object);
	}
}