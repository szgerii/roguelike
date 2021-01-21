#include <cmath>
#include "ShootyBoi.h"
#include "Engine/Game.h"

namespace CR::Entities {
	void ShootyBoi::tick() {
		const Vector2<float>& playerPos = Engine::getPlayer()->getPos();

		int xDiff = (int)round(playerPos.x) - (int)round(position.x),
			yDiff = (int)round(playerPos.y) - (int)round(position.y);
		
		if (inventory.getCurrentItem()->getMagazine() == 0) {
			inventory.getCurrentItem()->addAmmo(inventory.getCurrentItem()->getMagazineSize());
			inventory.getCurrentItem()->reload();
		}

		if (yDiff == 0) {
			if (xDiff < 0)
				inventory.getCurrentItem()->fire(Direction::LEFT);
			else
				inventory.getCurrentItem()->fire(Direction::RIGHT);
		} else if (xDiff == 0) {
			if (yDiff < 0)
				inventory.getCurrentItem()->fire(Direction::UP);
			else
				inventory.getCurrentItem()->fire(Direction::DOWN);			
		} else {
			float hMove = xDiff > 0 ? min(xDiff, hWalk) : max(xDiff, -hWalk),
				  vMove = yDiff > 0 ? min(yDiff, vWalk) : max(yDiff, -vWalk);

			GameObject* collObj = Engine::objectFromCoord({ (int)round(position.x + hMove), (int)round(position.y + vMove) });
			if (collObj != nullptr && collObj->getType() == Type::TILE) {
				int collObjXRounded = (int)round(collObj->getX()), collObjYRounded = (int)round(collObj->getY());

				if (collObjXRounded != (int)round(position.x)) {
					if (xDiff < 0)
						meleeAttack(Direction::LEFT);
					else if (xDiff > 0)
						meleeAttack(Direction::RIGHT);
				} else if (collObjYRounded != (int)round(position.y)) {
					if (yDiff < 0)
						meleeAttack(Direction::UP);
					else if (yDiff > 0)
						meleeAttack(Direction::DOWN);
				}
			}

			move({ hMove, vMove });
		}
	}
}