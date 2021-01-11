#pragma once
#include "Engine/GameObject.h"
#include "Events/KeyEvent.h"
//#include "Entity/Player.h"
#include "Utils.h"
#include <string>

namespace CR {
	enum Direction : char {
		LEFT, UP, RIGHT, DOWN
	};
}

namespace CR::Engine {
	void start();
	void generateRandomRoom();
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
}