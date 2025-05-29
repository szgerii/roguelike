#include <Windows.h>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <array>
#include <vector>
#include <algorithm>
#include <cmath>
#include <queue>
#include <utility>
#include <time.h>
#include <string>
#include <unordered_map>
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
#include "Object/Tiles/WeaponTile.h"
#include "Entity/Player.h"
#include "Entity/Enemies/Pavaka.h"
#include "Entity/Enemies/ShootyBoi.h"
#include "Entity/Enemies/Turret.h"
#include "Weapon/Weapon.h"
#include "Weapon/BoomBoomPistol.h"
#include "Weapon/Soti.h"
#include "Weapon/LongShot.h"
#include "Weapon/Fridge.h"
#include "Events/KeyEvent.h"
#include "Menus/MainMenu.h"
#include "Menus/PauseMenu.h"
#include "Menus/GameOverMenu.h"
#include "Menus/UpgradeMenu.h"
#include "Engine/Wave.h"

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

// Room Generation
static CR::Difficulty difficulty;
static CR::MapGenerationSetting mapGenerationSetting;
static std::vector<CR::Entities::Enemy*> enemies;
static std::vector<CR::Objects::PDestroyableTile*> pTiles;
static std::vector<CR::Weapons::Weapon*> weapons;
static std::vector<CR::Wave*> waves;
static int roomCount = 0, waveIndex = 0, lastWeaponTile = -1, wavesSinceLastShop;
static bool firstMove = true, genNextLevel = false, nextWave = false, shopMaxedOut = false;

static CR::GameObject* collisionMap[mapWidth][mapHeight];

static std::unordered_map<char, bool> inputMap;
bool ctrlActive = false, shiftActive = false, altActive = false;

// DEBUG (TODO: remove)
static int fpsCounter, tpsCounter, dropsCounter;

// DEBUG (TODO: remove)
static int keyPressDisplay;
static std::vector<char> pressedKeys;

static bool running = true, paused = false, backToMainMenu = false, quit = false;

static void generateRandomRoom();
static void handleGameObjects();

static void writeStringToScreenBuffer(std::string str, unsigned short color, int x, int y) {
	for (size_t i = 0; i < str.length(); i++)
		screen[y][x + i] = CHAR_INFO{ (wchar_t)str[i], color };
}

static void processInput() {
	DWORD numOfInputsAvailable, numOfInputsRead;
	GetNumberOfConsoleInputEvents(inputBuffer, &numOfInputsAvailable);

	INPUT_RECORD* inputs = new INPUT_RECORD[numOfInputsAvailable];
	ReadConsoleInputA(inputBuffer, inputs, numOfInputsAvailable, &numOfInputsRead);
	for (size_t i = 0; i < numOfInputsRead; i++) {
		if (inputs[i].EventType != KEY_EVENT || inputs[i].Event.KeyEvent.uChar.AsciiChar == '\0')
			continue;

		CR::KeyEvent ke(inputs[i].Event.KeyEvent);

		if (ke.keyDown) {
			if (ke.keyCode == 27) {
				CR::Menus::PauseMenu menu = CR::Menus::PauseMenu<mapWidth, mapHeight>(screenBuffer, inputBuffer);
				paused = true;
				int result = menu.show();

				switch (result) {
				case menu.Results::CONTINUE:
					break;

				case menu.Results::BACK_TO_MAIN_MENU:
					running = false;
					backToMainMenu = true;
					break;

				case menu.Results::QUIT:
					running = false;
					quit = true;
					break;
				}
			} else if (firstMove) {
				for (auto& enemy : enemies)
					enemy->setFreeze(false);

				for (auto& pTile : pTiles)
					CR::Engine::removeGameObject(pTile, true);

				firstMove = false;
			}

			auto b = std::find(pressedKeys.begin(), pressedKeys.end(), tolower(ke.keyValue));
			if (b == pressedKeys.end()) {
				pressedKeys.push_back(tolower(ke.keyValue));
			}
		}

		inputMap[tolower(ke.keyValue)] = ke.keyDown;

		unsigned int cks = inputs[i].Event.KeyEvent.dwControlKeyState;
		
		ctrlActive = (cks & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) > 0;
		shiftActive = (cks & SHIFT_PRESSED) > 0;
		altActive = (cks & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) > 0;
	}

	delete[] inputs;
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
		if (!object->getFreeze())
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
			if (overlay[i][j].Char.AsciiChar == '\0')
				continue;
			
			bool hasBG = (overlay[i][j].Attributes & (unsigned short)BG_WHITE) != 0;
			
			if (hasBG) {
				screen[i][j] = overlay[i][j];
			} else {
				// Merge
				bool fgRed = (overlay[i][j].Attributes & FOREGROUND_RED) != 0;
				bool fgGreen = (overlay[i][j].Attributes & FOREGROUND_GREEN) != 0;
				bool fgBlue = (overlay[i][j].Attributes & FOREGROUND_BLUE) != 0;
				bool fgIntensity = (overlay[i][j].Attributes & FOREGROUND_INTENSITY) != 0;
				screen[i][j].Char.AsciiChar = overlay[i][j].Char.AsciiChar;

				if (fgRed)
					screen[i][j].Attributes |= FOREGROUND_RED;
				else
					screen[i][j].Attributes &= ~FOREGROUND_RED;

				if (fgGreen)
					screen[i][j].Attributes |= FOREGROUND_GREEN;
				else
					screen[i][j].Attributes &= ~FOREGROUND_GREEN;

				if (fgBlue)
					screen[i][j].Attributes |= FOREGROUND_BLUE;
				else
					screen[i][j].Attributes &= ~FOREGROUND_BLUE;

				if (fgIntensity)
					screen[i][j].Attributes |= FOREGROUND_INTENSITY;
				else
					screen[i][j].Attributes &= ~FOREGROUND_INTENSITY;
			}
		}
	}

	int offset = 0;
	for (auto& text : guiTexts) {
		if (offset == 0) {
			writeStringToScreenBuffer(text, FG_WHITE, offset, mapHeight - 1);
			offset += text.length();
		} else if (text != "") {
			writeStringToScreenBuffer(" | " + text, FG_WHITE, offset, mapHeight - 1);
			offset += text.length() + 3;
		}
	}

	// DEBUG (TODO: remove)
	std::string debugStats = std::to_string(tpsCounter) + "/" + std::to_string(fpsCounter) + "/" + std::to_string(dropsCounter);
	writeStringToScreenBuffer(debugStats, FG_WHITE, mapWidth - debugStats.length(), mapHeight - 1);

	SMALL_RECT sr = SMALL_RECT{ 0, 0, mapWidth, mapHeight };
	WriteConsoleOutputA(screenBuffer, screen[0], { mapWidth, mapHeight }, { 0, 0 }, &sr);
}

static void handleGameObjects() {
	while (!gameObjectAddQueue.empty()) {
		if (gameObjectAddQueue.front()->getType() == CR::GameObject::Type::ENEMY)
			enemies.push_back((CR::Entities::Enemy*)gameObjectAddQueue.front());

		gameObjects.push_back(gameObjectAddQueue.front());
		gameObjectAddQueue.pop();
	}

	while (!gameObjectRemoveQueue.empty()) {
		auto gameObjectsIter = std::find(gameObjects.begin(), gameObjects.end(), gameObjectRemoveQueue.front().first);

		if (gameObjectsIter != gameObjects.end()) {
			gameObjects.erase(gameObjectsIter);

			if (gameObjectRemoveQueue.front().second)
				delete gameObjectRemoveQueue.front().first;
		}

		auto enemiesIter = std::find(enemies.begin(), enemies.end(), gameObjectRemoveQueue.front().first);
		if (enemiesIter != enemies.end())
			enemies.erase(enemiesIter);

		gameObjectRemoveQueue.pop();
	}
}

static void roomCleanUp(bool partial = false) {
	((CR::Entities::Player*)player)->deleteHealthbar();
	handleGameObjects();

	for (auto& obj : gameObjects)
		if (obj != player && (!partial || obj->getType() != CR::GameObject::Type::TILE))
			CR::Engine::removeGameObject(obj, true);
	((CR::Entities::Player*)player)->createHealthbar();
	handleGameObjects();

	enemies.clear();
	inputMap.clear();
	ctrlActive = false;
	shiftActive = false;
	altActive = false;

	for (size_t i = 0; i < mapWidth; i++) {
		for (size_t j = 0; j < mapHeight; j++) {
			collisionMap[i][j] = nullptr;
		}
	}
}

static void gameLoop() {
	using hrclock = std::chrono::high_resolution_clock;

	long long nsPerUpdate = (long long) (round(1000 / fps) * 1000000);
	long long lag = 0;
	hrclock::time_point lastTime = hrclock::now();

	// Debug (TODO: remove)
	int fpsTemp = 0, tpsTemp = 0, dropsTemp = 0;
	hrclock::time_point lastFpsCheck = hrclock::now();

	while (running) {
		hrclock::time_point currentTime = hrclock::now();
		long long elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(currentTime - lastTime).count();
		
		if (!paused)
			lag += elapsed;
		else
			paused = false;

		lastTime = currentTime;

		if (lag > nsPerUpdate * 2)
			dropsTemp++;

		processInput();
		while (lag > nsPerUpdate) {
			lag -= nsPerUpdate;
			updateCollision();
			tick();
			handleGameObjects();
			tpsTemp++;
		}
		render();

		if (nextWave) {
			roomCleanUp(true);
			waveIndex++;
			wavesSinceLastShop++;
			firstMove = true;

			if (waveIndex == waves.size())
				genNextLevel = true;
			else
				waves[waveIndex]->activate();

			nextWave = false;
		}

		if (genNextLevel) {
			generateRandomRoom();
			genNextLevel = false;
		}

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

static void generateRandomRoom() {
	roomCleanUp();

	waves.clear();
	waveIndex = 0;
	roomCount++;
	lastWeaponTile++;
	CR::Entities::Player* castPlayer = (CR::Entities::Player*)player;

	if (!shopMaxedOut && (wavesSinceLastShop >= 3 || (wavesSinceLastShop >= 2 && CR::getRandomNumberBetween(0, 1) == 0))) {
		CR::Menus::UpgradeMenu<mapWidth, mapHeight> sm(screenBuffer, inputBuffer);

		switch (sm.show()) {
		case sm.MAX_HEALTH:
			if (castPlayer->getMaxHealthProp().canUltimateUpgrade())
				castPlayer->getMaxHealthProp().upgrade(true);
			else
				castPlayer->getMaxHealthProp().upgrade();
			castPlayer->heal(castPlayer->getMaxHealth());
			break;

		case sm.MELEE_DAMAGE:
			if (castPlayer->getMeleeDamageProp().canUltimateUpgrade())
				castPlayer->getMeleeDamageProp().upgrade(true);
			else
				castPlayer->getMeleeDamageProp().upgrade();
			break;

		case sm.WEAPON_DAMAGE:
			castPlayer->upgradeWeaponDamage();
			break;

		case sm.DAMAGE_REDUCTION:
			if (castPlayer->getDamageReductionProp().canUltimateUpgrade())
				castPlayer->getDamageReductionProp().upgrade(true);
			else
				castPlayer->getDamageReductionProp().upgrade();
			break;

		case sm.AMMO_PICKUP:
			castPlayer->upgradePickupModifier();
			break;

		case sm.CANCEL:
			shopMaxedOut = true;
			break;
		}
		
		wavesSinceLastShop = 0;
	}

	int noiseDensity = -1, iteration = -1, wallReq = -1, floorReq = -1;

	using CR::MapGenerationSetting;
	switch (mapGenerationSetting) {
	case MapGenerationSetting::EMPTY:
		noiseDensity = 0;
		iteration = 0;
		wallReq = 0;
		floorReq = 0;
		break;

	case MapGenerationSetting::OPEN:
		noiseDensity = 24;
		iteration = 9;
		wallReq = 4;
		floorReq = 6;
		break;

	case MapGenerationSetting::NORMAL:
		noiseDensity = 30;
		iteration = 8;
		wallReq = 4;
		floorReq = 6;
		break;

	case MapGenerationSetting::TIGHT:
		noiseDensity = 37;
		iteration = 7;
		wallReq = 4;
		floorReq = 6;
		break;

	case MapGenerationSetting::FULL:
		noiseDensity = 100;
		iteration = 0;
		wallReq = 0;
		floorReq = 0;
		break;
	}

	// 0 - floor
	// 1 - wall
	// 2 - start point
	// 3 - tile
	// 4 - enemy
	// 5 - health
	// 6 - ammo
	// 255 - etc
	unsigned char room[mapWidth][mapHeight] = {};

	// Border
	for (int y = 0; y < mapHeight - 1; y++) {
		if (y == 0 || y == mapHeight - 2) {
			for (int x = 0; x < mapWidth; x++)
				room[x][y] = 3;

			continue;
		}

		room[0][y] = 3;
		room[mapWidth - 1][y] = 3;
	}

	// Walls
	for (int x = 1; x < mapWidth - 1; x++)
		for (int y = 1; y < mapHeight - 2; y++)
			room[x][y] = rand() % 100 < noiseDensity ? 1 : 0;

	for (int i = 0; i < iteration; i++) {
		for (int x = 1; x < mapWidth - 1; x++) {
			for (int y = 1; y < mapHeight - 2; y++) {
				int adjacentWalls = 0;

				for (int j = x - 1; j <= x + 1; j++) {
					for (int k = y - 1; k <= y + 1; k++) {
						if (j == x && k == y)
							continue;

						if (room[j][k] == 1 || room[j][k] == 3)
							adjacentWalls++;
					}
				}

				int adjacentFloors = 8 - adjacentWalls;

				if (room[x][y] == 0 && adjacentFloors < wallReq)
					room[x][y] = 1;
				else if (room[x][y] == 1 && adjacentFloors >= floorReq)
					room[x][y] = 0;
			}
		}
	}

	int waveCount = -1;
	switch (difficulty) {
	case CR::Difficulty::EASY:
		waveCount = min((int)(roomCount / 10) + 1, 5);
		break;

	case CR::Difficulty::NORMAL:
		waveCount = min((int)(roomCount / 5) + 1, 7);
		break;

	case CR::Difficulty::HARD:
		waveCount = min((int)(roomCount / 3) + 1, 8);
		break;

	case CR::Difficulty::VERY_HARD:
		waveCount = min((int)round(roomCount / 1.8) + 1, 10);
		break;
	}

	if (difficulty == CR::Difficulty::VERY_HARD)
		castPlayer->heal(castPlayer->getMaxHealth());

	int enemyPerWave = -1;
	switch (difficulty) {
	case CR::Difficulty::EASY:
		enemyPerWave = min((int)(roomCount / 5) + 1, 5);
		break;

	case CR::Difficulty::NORMAL:
		enemyPerWave = min((int)(roomCount / 3) + 1, 7);
		break;

	case CR::Difficulty::HARD:
		enemyPerWave = min((int)(roomCount / 2) + 1, 9);
		break;

	case CR::Difficulty::VERY_HARD:
		enemyPerWave = min((int)round(roomCount / 1.4) + 1, 11);
		break;
	}

	int ammoTileCount = -1;
	switch (difficulty) {
	case CR::Difficulty::EASY:
		ammoTileCount = min(enemyPerWave, 5);
		break;

	case CR::Difficulty::NORMAL:
		ammoTileCount = min((int)round(enemyPerWave / 2.2), 3);
		break;

	case CR::Difficulty::HARD:
		ammoTileCount = min((int)round(enemyPerWave / 2.8), 3);
		break;

	case CR::Difficulty::VERY_HARD:
		ammoTileCount = min((int)round(enemyPerWave / 3), 4);
		break;
	}
	if (ammoTileCount < 1)
		ammoTileCount = 1;

	int healthTileCount = -1;
	switch (difficulty) {
	case CR::Difficulty::EASY:
		healthTileCount = min((int)round(enemyPerWave / 2.2), 2);
		break;

	case CR::Difficulty::NORMAL:
		healthTileCount = min((int)round(enemyPerWave / 2.6), 3);
		break;

	case CR::Difficulty::HARD:
		healthTileCount = min((int)round(enemyPerWave / 3.1), 3);
		break;

	case CR::Difficulty::VERY_HARD:
		healthTileCount = 0;
		break;
	}
	if (difficulty != CR::Difficulty::VERY_HARD && healthTileCount < 1)
		healthTileCount = 1;

	int weaponDropChance = 0;
	switch (difficulty) {
	case CR::Difficulty::EASY:
		weaponDropChance = min(lastWeaponTile * 30, 80);
		break;

	case CR::Difficulty::NORMAL:
		weaponDropChance = min(lastWeaponTile * 20, 70);
		break;

	case CR::Difficulty::HARD:
		weaponDropChance = min(lastWeaponTile * 15, 65);
		break;

	case CR::Difficulty::VERY_HARD:
		weaponDropChance = min(lastWeaponTile * 12, 50);
		break;
	}
	bool dropWeaponTile = CR::getRandomNumberBetween(1, 100) <= weaponDropChance;
	if (dropWeaponTile)
		lastWeaponTile = -1;

	// Player
	int playerX = CR::getRandomNumberBetween(3, mapWidth / 4), playerY = CR::getRandomNumberBetween(3, mapHeight - 5);
	for (int i = playerX - 2; i <= playerX + 2; i++)
		for (int j = playerY - 2; j <= playerY + 2; j++)
			room[i][j] = (i == playerX - 2 || i == playerX + 2 || j == playerY - 2 || j == playerY + 2) ? 0 : 0;

	room[playerX][playerY] = 2;
	castPlayer->moveTo({ (float)playerX, (float)playerY });

	// End tile
	int x, y;
	do {
		x = CR::getRandomNumberBetween((int)round(mapWidth * 0.8), mapWidth - 2);
		y = CR::getRandomNumberBetween(1, mapHeight - 3);
	} while (room[x][y] != 0 && room[x][y] != 1);
	room[x][y] = 255;
	gameObjects.push_back(new CR::Objects::EndTile({ x, y }));

	// Random weapon
	CR::Weapons::Weapon* weapon = nullptr;
	if (dropWeaponTile && weapons.size() != 0) {
		int index = CR::getRandomNumberBetween(0, weapons.size() - 1);
		weapon = weapons[index];
		weapons.erase(weapons.begin() + index);
	}

	// actually generate the map
	for (int x = 0; x < mapWidth; x++) {
		for (int y = 0; y < mapHeight - 1; y++) {
			CR::GameObject* obj = nullptr;
			CR::GameObject* occupant = CR::Engine::objectFromCoord({ x, y });
			bool skip = false;

			switch (room[x][y]) {
			case 1:
				if (occupant != nullptr && occupant->getType() == CR::GameObject::Type::TILE) {
					continue;
				}
				obj = new CR::Objects::DestroyableTile(' ', BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN, { x, y }, 1);
				break;

			case 3:
				obj = new CR::Objects::Tile(' ', BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN, { x, y });
				break;

			default:
				skip = true;
				break;
			}

			if (skip)
				continue;

			CR::Engine::addGameObject(obj);
		}
	}

	for (int i = 0; i < waveCount; i++)
		waves.push_back(new CR::Wave(enemyPerWave, ammoTileCount, healthTileCount, room[0], weapon));

	waves[0]->activate();

	firstMove = true;
	handleGameObjects();
}

static void showMainMenu() {
	CR::Menus::MainMenu menu = CR::Menus::MainMenu<mapWidth, mapHeight>(screenBuffer, inputBuffer);
	int result = menu.show();

	switch (result) {
	case menu.Results::START:
		break;

	case menu.Results::TUTORIAL:
		break;

	case menu.Results::QUIT:
		running = false;
		quit = true;
		break;
	}
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

		showMainMenu();

		bool gameRunning = true;

		while (gameRunning) {
			for (auto& obj : gameObjects)
				removeGameObject(obj, true);

			guiTexts.clear();
			
			weapons.push_back(new Weapons::BoomBoomPistol());
			weapons.push_back(new Weapons::Soti(12.0f, 4.0f));
			weapons.push_back(new Weapons::LongShot(4.0f, 12.0f));
			weapons.push_back(new Weapons::Fridge(3000, 0.76f, 0.4f));
			weapons.push_back(new Weapons::Weapon("The Slow Burn", 13.0f, 0.2f, 0.16f, 1000, 10, 30, 8));
			weapons.push_back(new Weapons::Weapon("The BRRRRR", 2, 0.9f, 0.58f, 112, 45, 320, 35));

			auto player = new Entities::Player({ 30, 30 });
			player->pickUpItem(new Weapons::Weapon("The Generic", 5, 0.8f, 0.33f, 400, 20, 56, 20));
			addGameObject(player);
			setPlayer(player);

			if (!quit)
				running = true;

			roomCount = 0;
			wavesSinceLastShop = 0;

			generateRandomRoom();
			gameLoop();

			if (quit) {
				gameRunning = false;
			} else if (backToMainMenu) {
				showMainMenu();
				backToMainMenu = false;
			} else {
				Menus::GameOverMenu gom = Menus::GameOverMenu<mapWidth, mapHeight>(screenBuffer, inputBuffer);
				int result = gom.show();

				switch (result) {
				case gom.Results::RESTART:
					break;

				case gom.Results::QUIT:
					gameRunning = false;
					break;
				}
			}
		}
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
		return GetKeyState(toupper(keyValue)) & 0x8000;
	}

	bool ctrlPressed() {
		return GetKeyState(VK_CONTROL) & 0x8000;
	}
	
	bool shiftPressed() {
		return GetKeyState(VK_SHIFT) & 0x8000;
	}
	
	bool altPressed() {
		return GetKeyState(VK_MENU) & 0x8000;
	}

	int addGUIText(std::string text) {
		guiTexts.push_back(text);
		return guiTexts.size() - 1;
	}

	void modifyGUIText(int index, std::string text) {
		guiTexts[index] = text;
	}

	void addToOverlay(const Vector2<int>& pos, char ch, unsigned short color) {
		if (pos.x < 0 || pos.x >= mapWidth || pos.y < 0 || pos.y >= mapHeight) {
			return;
		}

		overlay[pos.y][pos.x] = CHAR_INFO{ (wchar_t)ch, color };
	}

	void gameOver() {
		running = false;
	}

	void nextLevel() {
		nextWave = true;
	}

	MapGenerationSetting getMapGenerationSetting() {
		return mapGenerationSetting;
	}

	void setMapGenerationSetting(MapGenerationSetting mgs) {
		mapGenerationSetting = mgs;
	}

	int getNumberOfEnemies() {
		return enemies.size();
	}
	
	Difficulty getDifficulty() {
		return difficulty;
	}

	void setDifficulty(Difficulty diff) {
		difficulty = diff;
	}

	int getRoomCount() {
		return roomCount;
	}

	int getCurrentWaveNum() {
		return waveIndex + 1;
	}

	int getNumOfWaves() {
		return waves.size();
	}
}