#include <Windows.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <utility>
#include <time.h>
#include <string>
#include <map>
#include <ctype.h>
#include "Engine/Game.h"
#include "Engine/GameObject.h"
#include "Engine/CustomGameObject.h"
#include "Object/Tiles/AmmoTile.h"
#include "Object/Tiles/HealthTile.h"
#include "Object/Tiles/EndTile.h"
#include "Object/Tiles/DestroyableTile.h"
#include "Object/Tiles/PDestroyableTile.h"
#include "Entity/Player.h"
#include "Events/KeyEvent.h"

const static float fps = 80;
const static unsigned short mapWidth = 120, mapHeight = 40;
const static unsigned int mapArea = mapWidth * mapHeight;
static HANDLE screenBuffer, inputBuffer;
static CHAR_INFO screen[mapHeight][mapWidth];
static CHAR_INFO overlay[mapHeight][mapWidth];
static std::vector<std::string> guiTexts;

static std::vector<CR::GameObject*> gameObjects;
static CR::GameObject* player;
static std::queue<CR::GameObject*> gameObjectAddQueue;
static std::queue<std::pair<CR::GameObject*, bool>> gameObjectRemoveQueue;

static CR::GameObject* collisionMap[mapWidth][mapHeight];

static std::unordered_map<char, bool> inputMap;
bool ctrlActive = false, shiftActive = false, altActive = false;

// DEBUG (TODO: remove)
static int fpsCounter, tpsCounter, dropsCounter;

static bool running = true;

static void writeStringToScreenBuffer(std::string str, unsigned short color, int x, int y) {
	for (size_t i = 0; i < str.length(); i++)
		screen[y][x + i] = CHAR_INFO{ (wchar_t)str[i], color };
}

static void processInput() {
	DWORD numOfInputsAvailable, numOfInputsRead;
	INPUT_RECORD* inputs = nullptr;

	// Input processing
	GetNumberOfConsoleInputEvents(inputBuffer, &numOfInputsAvailable);

	if (inputs != nullptr)
		delete[] inputs;
	inputs = new INPUT_RECORD[numOfInputsAvailable];

	ReadConsoleInputA(inputBuffer, inputs, numOfInputsAvailable, &numOfInputsRead);
	for (size_t i = 0; i < numOfInputsRead; i++) {
		if (inputs[i].EventType != KEY_EVENT || inputs[i].Event.KeyEvent.uChar.AsciiChar == '\0')
			continue;

		CR::KeyEvent ke(inputs[i].Event.KeyEvent);

		if (ke.keyValue == 'n' && ke.keyDown)
			CR::Engine::generateRandomRoom();

		inputMap[tolower(ke.keyValue)] = ke.keyDown;

		unsigned int cks = inputs[i].Event.KeyEvent.dwControlKeyState;

		ctrlActive = (cks & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) > 0;
		shiftActive = (cks & SHIFT_PRESSED) > 0;
		altActive = (cks & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) > 0;

		if (ke.keyValue == 'u')
			running = false;

		for (size_t i = 0; i < gameObjects.size(); i++)
			gameObjects[i]->onKeyEvent(ke);
	}
}

static void updateCollision() {
	for (size_t i = 0; i < mapWidth; i++)
		for (size_t j = 0; j < mapHeight; j++)
			collisionMap[i][j] = nullptr;

	for (auto& object : gameObjects)
		object->updateCollision();
}

static void tick() {
	for (size_t i = 0; i < mapHeight; i++)
		for (size_t j = 0; j < mapWidth; j++)
			overlay[i][j] = CHAR_INFO();

	for (auto& object : gameObjects)
		object->tick();
}

static void render() {
	for (size_t i = 0; i < mapHeight; i++)
		for (size_t j = 0; j < mapWidth; j++)
			screen[i][j] = CHAR_INFO();

	for (auto& object : gameObjects) {
		if (object->customObject)
			((CR::CustomGameObject*) object)->render(screen[0], mapWidth, mapHeight);
		else
			screen[(int)round(object->getY())][(int)round(object->getX())] = CHAR_INFO{ (wchar_t)object->getSkin(), object->getSkinColor() };
	}

	for (size_t i = 0; i < mapHeight; i++) {
		for (size_t j = 0; j < mapWidth; j++) {
			if (overlay[i][j].Char.AsciiChar != '\0')
				screen[i][j] = overlay[i][j];
		}
	}

	int offset = 0;
	for (auto& text : guiTexts) {
		if (offset == 0) {
			writeStringToScreenBuffer(text, WHITE, offset, mapHeight - 1);
			offset += text.length();
		} else {
			writeStringToScreenBuffer(" | " + text, WHITE, offset, mapHeight - 1);
			offset += text.length() + 3;
		}
	}

	// DEBUG (TODO: remove)
	std::string debugStats = std::to_string(tpsCounter) + "/" + std::to_string(fpsCounter) + "/" + std::to_string(dropsCounter);
	writeStringToScreenBuffer(debugStats, WHITE, mapWidth - debugStats.length(), mapHeight - 1);

	SMALL_RECT sr = SMALL_RECT{ 0, 0, mapWidth, mapHeight };
	WriteConsoleOutputA(screenBuffer, screen[0], { mapWidth, mapHeight }, { 0, 0 }, &sr);
}

static void handleGameObjects() {
	while (!gameObjectAddQueue.empty()) {
		gameObjects.push_back(gameObjectAddQueue.front());
		gameObjectAddQueue.pop();
	}

	while (!gameObjectRemoveQueue.empty()) {
		auto iter = std::find(gameObjects.begin(), gameObjects.end(), gameObjectRemoveQueue.front().first);

		if (iter != gameObjects.end()) {
			gameObjects.erase(iter);

			if (gameObjectRemoveQueue.front().second)
				delete gameObjectRemoveQueue.front().first;
		}

		gameObjectRemoveQueue.pop();
	}
}

static void gameLoop() {
	using hrclock = std::chrono::high_resolution_clock;

	long long msPerUpdate = (long long) round(1000 / fps);
	long long lag = 0;
	hrclock::time_point lastTime = hrclock::now();

	// Debug (TODO: remove)
	int fpsTemp = 0, tpsTemp = 0, dropsTemp = 0;
	hrclock::time_point lastFpsCheck = hrclock::now();

	while (running) {
		hrclock::time_point currentTime = hrclock::now();
		long long elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();
		lag += elapsed;
		lastTime = currentTime;

		if (lag > msPerUpdate * 2)
			dropsTemp++;

		processInput();
		while (lag > msPerUpdate) {
			updateCollision();
			tick();
			handleGameObjects();
			lag -= msPerUpdate;
			tpsTemp++;
		}
		render();
		fpsTemp++;

		if (std::chrono::duration_cast<std::chrono::milliseconds>(hrclock::now() - lastFpsCheck).count() > 1000) {
			lastFpsCheck = hrclock::now();

			tpsCounter = tpsTemp;
			fpsCounter = fpsTemp;
			dropsCounter = dropsTemp;
			tpsTemp = 0;
			fpsTemp = 0;
			dropsTemp = 0;
		}
	}
}

static std::vector<CR::GameObject*> roomObjects;

// min, max inclusive
static int getRandomNumberBetween(int min, int max) {
	return (rand() % (max - min + 1)) + min;	
}

namespace CR::Engine {
	void start() {
		screenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		inputBuffer = GetStdHandle(STD_INPUT_HANDLE);

		SetConsoleScreenBufferSize(screenBuffer, { mapWidth, mapHeight });
		SetConsoleActiveScreenBuffer(screenBuffer);

		CONSOLE_SCREEN_BUFFER_INFO* scbi = new CONSOLE_SCREEN_BUFFER_INFO();
		GetConsoleScreenBufferInfo(screenBuffer, scbi);
		SMALL_RECT conSize = SMALL_RECT{ 0, 0, scbi->dwMaximumWindowSize.X - 1, scbi->dwMaximumWindowSize.Y - 1 };
		SetConsoleWindowInfo(screenBuffer, TRUE, &conSize);
		delete scbi;

		SetConsoleTitleA("Roguelike");

		CONSOLE_CURSOR_INFO* ci = new CONSOLE_CURSOR_INFO{ 1, FALSE };
		SetConsoleCursorInfo(screenBuffer, ci);
		delete ci;

		gameLoop();

		// TODO: clean up
	}

	void generateRandomRoom() {
		for (auto& obj : roomObjects)
			removeGameObject(obj, true);

		handleGameObjects();

		roomObjects.clear();

		// 0 - floor
		// 1 - wall
		// 2 - start point
		// 3 - end tile
		// 4 - ammo tile
		// 5 - health tile
		// 6 - pdestructable tile (only the player can destroy it)
		char room[mapWidth][mapHeight] = {};

		// Border
		for (int y = 0; y < mapHeight - 1; y++) {
			if (y == 0 || y == mapHeight - 2) {
				for (int x = 0; x < mapWidth; x++)
					room[x][y] = 1;
				
				continue;
			}
			
			room[0][y] = 1;
			room[mapWidth - 1][y] = 1;
		}

		// Walls
		for (int x = 1; x < mapWidth - 1; x++)
			for (int y = 1; y < mapHeight - 2; y++)
				room[x][y] = rand() % 100 < 31 ? 1 : 0;

		for (int i = 0; i < 5; i++) {
			for (int x = 1; x < mapWidth - 1; x++) {
			for (int y = 1; y < mapHeight - 2; y++) {
				int adjacentWalls = 0;

				for (int j = x - 1; j <= x + 1; j++) {
					for (int k = y - 1; k <= y + 1; k++) {
						if (j == x && k == y)
							continue;

						if (room[j][k] == 1)
							adjacentWalls++;
					}
				}

				int adjacentFloors = 8 - adjacentWalls;

				if (room[x][y] == 0 && adjacentFloors < 4)
					room[x][y] = 1;
				else if (room[x][y] == 1 && adjacentFloors >= 6)
					room[x][y] = 0;
			}
			}
		}

		// Player
		int playerX = getRandomNumberBetween(3, mapWidth / 4), playerY = getRandomNumberBetween(3, mapHeight - 4);
		for (int i = playerX - 2; i <= playerX + 2; i++)
			for (int j = playerY - 2; j <= playerY + 2; j++)
				room[i][j] = (i == playerX - 2 || i == playerX + 2 || j == playerY - 2 || j == playerY + 2 ) ? 6 : 0;

		room[playerX][playerY] = 2;
		((Entities::Player*)getPlayer())->moveTo({ (float)playerX, (float)playerY });

		// End tile
		int endX = getRandomNumberBetween(mapWidth / 4 * 3, mapWidth - 2), endY = getRandomNumberBetween(3, mapHeight - 3);
		room[endX][endY] = 3;

		room[playerX][playerY] = 2;
		((Entities::Player*)getPlayer())->moveTo({ (float)playerX, (float)playerY });

		// Ammo tiles and health tiles
		int ammoTiles = 5, healthTiles = 2;
		while (ammoTiles > 0 || healthTiles > 0) {
			int x = (rand() % (mapWidth - 2)) + 1;
			int y = (rand() % (mapHeight - 3)) + 1;

			if (room[x][y] != 0 && room[x][y] != 1)
				continue;

			if (ammoTiles > 0) {
				room[x][y] = 4;
				ammoTiles--;
			} else {
				room[x][y] = 5;
				healthTiles--;
			}
		}

		// actually generate the map
		for (int x = 1; x < mapWidth - 1; x++) {
			for (int y = 1; y < mapHeight - 2; y++) {
				GameObject* obj;
				bool skip = false;

				switch (room[x][y]) {
				case 1:
					obj = new Objects::DestroyableTile(' ', BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN, { x, y }, 1);
					break;

				case 3:
					obj = new Objects::EndTile({ x, y });
					break;

				case 4:
					obj = new Objects::AmmoTile({ x, y }, 10);
					break;
				
				case 5:
					obj = new Objects::HealthTile({ x, y }, 10);
					break;

				case 6:
					obj = new Objects::PDestroyableTile(' ', BACKGROUND_RED, { x, y }, 1);
					break;
				
				default:
					skip = true;
					break;
				}

				if (skip)
					continue;

				roomObjects.push_back(obj);
				addGameObject(obj);
			}
		}

		handleGameObjects();
	}

	void addGameObject(GameObject* object) {
		gameObjectAddQueue.push(object);
	}

	void removeGameObject(GameObject* object, bool deleteObj) {
		gameObjectRemoveQueue.push({ object, deleteObj });
	}

	void setRandomSeed(int seed) {
		srand(seed);
	}

	void setPlayer(GameObject* p) {
		player = p;
	}

	const GameObject* getPlayer() {
		return player;
	}

	int getMapWidth() {
		return mapWidth;
	}

	int getMapHeight() {
		return mapHeight;
	}

	void addCollision(const Vector2<int>& coord, GameObject* object) {
		collisionMap[coord.x][coord.y] = object;
	}

	void addCollision(int x, int y, GameObject* object) {
		collisionMap[x][y] = object;
	}

	bool collisionAtCoord(const Vector2<int>& coord, const GameObject* ignoreObj) {
		return collisionMap[coord.x][coord.y] != nullptr && collisionMap[coord.x][coord.y] != ignoreObj;
	}
	
	bool collisionAtCoord(int x, int y, const GameObject* ignoreObj) {
		return collisionMap[x][y] != nullptr && collisionMap[x][y] != ignoreObj;
	}

	GameObject* objectFromCoord(const Vector2<int>& coord) {
		return collisionMap[coord.x][coord.y];
	}

	bool keyPressed(char keyValue) {
		return inputMap[tolower(keyValue)];
	}

	bool ctrlPressed() {
		return ctrlActive;
	}
	
	bool shiftPressed() {
		return shiftActive;
	}
	
	bool altPressed() {
		return altActive;
	}

	int addGUIText(std::string text) {
		guiTexts.push_back(text);
		return guiTexts.size() - 1;
	}

	void modifyGUIText(int index, std::string text) {
		guiTexts[index] = text;
	}

	void addToOverlay(int x, int y, char ch, unsigned short color) {
		overlay[y][x] = CHAR_INFO{ (wchar_t)ch, color };
	}
}