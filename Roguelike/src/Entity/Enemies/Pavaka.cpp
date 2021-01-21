#include <cmath>
#include "Pavaka.h"
#include "Utils.h"

namespace CR::Entities {
	void Pavaka::tick() {
		Vector2<float> playerPos = Engine::getPlayer()->getPos();

		int playerDeltaX = (int)round(playerPos.x) - (int)round(position.x), playerDeltaY = (int)round(playerPos.y) - (int)round(position.y);

		if (playerDeltaX == 1 && (playerDeltaY <= 1 && playerDeltaY >= -1)) {
			meleeAttack(Direction::RIGHT);
		} else if (playerDeltaX == -1 && (playerDeltaY <= 1 && playerDeltaY >= -1)) {
			meleeAttack(Direction::LEFT);
		} else if (playerDeltaY == 1 && (playerDeltaX <= 1 && playerDeltaX >= -1)) {
			meleeAttack(Direction::DOWN);
		} else if (playerDeltaY == -1 && (playerDeltaX <= 1 && playerDeltaX >= -1)) {
			meleeAttack(Direction::UP);
		} else {
			if (playerPos != lastPlayerPos) {
				targetCell = position.getClosestCellTo(playerPos);
				lastPlayerPos = playerPos;
			}

			float hDiff = 0, vDiff = 0;

			int targetCellXRounded = (int)round(targetCell.x), targetCellYRounded = (int)round(targetCell.y),
				positionXRounded = (int)round(position.x), positionYRounded = (int)round(position.y);

			if (targetCellXRounded > positionXRounded)
				hDiff = hWalk;
			else if (targetCellXRounded < positionXRounded)
				hDiff = -hWalk;

			if (targetCellYRounded > positionYRounded)
				vDiff = vWalk;
			else if (targetCellYRounded < positionYRounded)
				vDiff = -vWalk;

			GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x + hDiff), (int)round(position.y + vDiff) });
			if (collObj != nullptr && collObj->getType() == Type::TILE) {
				int collObjXRounded = (int)round(collObj->getX()), collObjYRounded = (int)round(collObj->getY());

				if (collObjXRounded != positionXRounded) {
					if (hDiff < 0)
						meleeAttack(Direction::LEFT);
					else if (hDiff > 0)
						meleeAttack(Direction::RIGHT);
				} else if (collObjYRounded != positionYRounded) {
					if (vDiff < 0)
						meleeAttack(Direction::UP);
					else if (vDiff > 0)
						meleeAttack(Direction::DOWN);
				}

				return;
			}

			move({ hDiff, vDiff });
		}
	}
}