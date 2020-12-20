#include "Game.h"
#include "GameObject.h"

int main() {
	//CR::GameObject go('@', FOREGROUND_GREEN | FOREGROUND_BLUE, 0, 0);
	//CR::Engine::addGameObject(&go);

	for (size_t i = 0; i < 120; i++) {
		for (size_t j = 0; j < 40; j++) {
			CR::GameObject* go = nullptr;
			if (i % 2 == 0 == j % 2)
				go = new CR::GameObject(' ', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED, i, j);
			else
				go = new CR::GameObject(' ', BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED, i, j);
			
			CR::Engine::addGameObject(go);
		}
	}

	CR::Engine::start();
}