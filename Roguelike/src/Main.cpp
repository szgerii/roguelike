#include "Game.h"
#include "GameObject.h"

int main() {
	CR::GameObject go('@', 0, 0);
	CR::Engine::addGameObject(&go);
	CR::Engine::start();
}