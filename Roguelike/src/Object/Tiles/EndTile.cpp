#include "EndTile.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void EndTile::tick() {
		if (Engine::getNumberOfEnemies() != 0)
			skin = 'X';
		else
			skin = ' ';
	}

	void EndTile::hurt(float amount, GameObject* from) {
		if (Engine::getNumberOfEnemies() == 0)
			Engine::nextLevel();
	}
}
