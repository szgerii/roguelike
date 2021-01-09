#include "HealthTile.h"
#include "Entity/Player.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void HealthTile::hurt(float amount, GameObject* from) {
		((Entities::Player*) Engine::getPlayer())->heal((float) healthAmount);

		die();
	}
}