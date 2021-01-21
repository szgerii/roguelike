#include <cmath>
#include "Healthbar.h"
#include "Engine/Game.h"

namespace CR::Particles {
	void Healthbar::render(CHAR_INFO* screen, int mapWidth, int mapHeight) const {
		Entities::Entity* trackedEntity = (Entities::Entity*)trackedObj;
		int healthStep = (int)round(trackedEntity->getMaxHealth() / 5);
		float currHealth = trackedEntity->getHealth();
		int posX = (int)round(trackedObj->getX()) + offset.x;
		int posY = (int)round(trackedObj->getY()) + offset.y;

		for (int i = (posX == 1 ? -1 : -2); i <= (posX == Engine::getMapWidth() - 2 ? 1 : 2); i++) {
			bool isActive = healthStep * (i + 2) >= currHealth;

			Engine::addToOverlay({ posX + i, posY }, skin, isActive ? activeColor : inactiveColor);
		}
	}
}