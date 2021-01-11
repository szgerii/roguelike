#include <cmath>
#include "Healthbar.h"
#include "Engine/Game.h"

namespace CR::Particles {
	void Healthbar::render(CHAR_INFO* screen, int mapWidth, int mapHeight) const {
		Entities::Entity* trackedEntity = (Entities::Entity*)trackedObj;
		int healthStep = round(trackedEntity->maxHealth / 5);
		int currHealth = trackedEntity->getHealth();

		for (int i = (position.x == 1 ? -1 : -2); i <= (position.x == Engine::getMapWidth() - 2 ? 1 : 2); i++) {
			bool isActive = healthStep * (i + 2) >= currHealth;

			Engine::addToOverlay({ (int)position.x + i, (int)position.y }, skin, isActive ? activeColor : inactiveColor);
		}
	}
}