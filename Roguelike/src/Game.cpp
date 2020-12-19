#include <Windows.h>
#include <thread>
#include <chrono>
#include "Game.h"
#include "GameObject.h"

namespace CR::Engine {
	std::vector<CR::GameObject*> gameObjects;

	unsigned int mapWidth = 120, mapHeight = 40;
	HANDLE console;
	wchar_t* screen = new wchar_t[mapWidth * mapHeight];

	void start() {
		console = CreateConsoleScreenBuffer(GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleActiveScreenBuffer(console);
		DWORD bytesWritten = 0;

		while (true) {
			gameObjects[0]->setX(gameObjects[0]->getX() + 1);

			for (size_t i = 0; i < mapHeight; i++) {
				for (size_t j = 0; j < mapWidth; j++) {
					screen[i * mapWidth + j] = ' ';
				}
			}

			for (auto& object : gameObjects) {
				screen[object->getY() * mapWidth + object->getX()] = object->getSkin();
			}

			WriteConsoleOutputCharacter(console, screen, mapWidth * mapHeight, { 0,0 }, &bytesWritten);

			using namespace std::literals::chrono_literals;
			std::this_thread::sleep_for(5ms);
		}
	}

	void addGameObject(CR::GameObject* object) {
		gameObjects.push_back(object);
	}
}