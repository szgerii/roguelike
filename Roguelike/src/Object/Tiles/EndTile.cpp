#include "EndTile.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void EndTile::hurt(float amount, GameObject* from) {
		Engine::generateRandomRoom();
	}
}
