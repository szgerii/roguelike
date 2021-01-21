#pragma once
#include <string>
#include "Engine/GameObject.h"
#include "Events/KeyEvent.h"
#include "Utils.h"

namespace CR {
	enum class Direction : char {
		LEFT, UP, RIGHT, DOWN
	};

	enum class MapGenerationSetting : char {
		EMPTY, OPEN, NORMAL, TIGHT, FULL
	};

	enum class Difficulty : char {
		EASY, NORMAL, HARD, VERY_HARD
	};
}

namespace CR::Engine {
	void start();
	void nextLevel();
	void addGameObject(CR::GameObject* object);
	void removeGameObject(CR::GameObject* object, bool deleteObj=false);
	void setRandomSeed(int seed);
	void setPlayer(GameObject* p);
	const GameObject* getPlayer();
	int getMapWidth();
	int getMapHeight();
	void addCollision(const Vector2<int>& coord, GameObject* object);
	void addCollision(int x, int y, GameObject* object);
	bool collisionAtCoord(const Vector2<int>& coord, const GameObject* ignoreObj);
	bool collisionAtCoord(int x, int y, const GameObject* ignoreObj);
	GameObject* objectFromCoord(const Vector2<int>& coord);
	bool keyPressed(char keyValue);
	bool ctrlPressed();
	bool shiftPressed();
	bool altPressed();
	int addGUIText(std::string stat);
	void modifyGUIText(int index, std::string stat);
	void addToOverlay(const Vector2<int>&, char ch, unsigned short color);
	void gameOver();
	void setMapGenerationSetting(MapGenerationSetting mgs);
	MapGenerationSetting getMapGenerationSetting();
	int getNumberOfEnemies();
	int getRoomCount();
	int getCurrentWaveNum();
	int getNumOfWaves();
	Difficulty getDifficulty();
	void setDifficulty(Difficulty diff);
}