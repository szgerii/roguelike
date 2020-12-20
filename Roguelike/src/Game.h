#pragma once
#include "GameObject.h"

namespace CR::Engine {
	enum Direction : char {
		LEFT, UP, RIGHT, DOWN
	};

	void start();
	void addGameObject(GameObject* object);
	void removeGameObject(GameObject* object);
}