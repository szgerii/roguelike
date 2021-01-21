#include "HealthTile.h"
#include "Entity/Player.h"
#include "Engine/Game.h"

namespace CR::Objects {
	void HealthTile::hurt(float amount, GameObject* from) {
		if (from->getType() == GameObject::Type::ENEMY || from->getType() == GameObject::Type::PLAYER)
			((Entities::Entity*) from)->heal((float) healthAmount);

		die();
	}
}