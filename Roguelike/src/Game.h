#pragma once
#include <vector>
#include "GameObject.h"

namespace CR::Engine {
	enum Direction : char {
		LEFT, UP, RIGHT, DOWN
	};

	void start();
	void tick();
	void render();
	void addGameObject(GameObject* object);
	void removeGameObject(GameObject* object);
}