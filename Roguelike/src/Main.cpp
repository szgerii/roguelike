#include <time.h>
#include "Engine/Game.h"
#include "Object/Walls/HollowRectWall.h"
#include "Entity/Player.h"
#include "Weapon/Weapon.h"

#include "Entity/Enemies/Enemy.h"
#include "Entity/Enemies/Pavaka.h"
#include "Entity/Enemies/Turret.h"
#include "Entity/Enemies/ShootyBoi.h"
#include "Weapon/BoomBoomPistol.h"
#include "Object/Tiles/WeaponTile.h"

using namespace CR;

int main() {
	Engine::setRandomSeed((int)time(NULL));

	Engine::setMapGenerationSetting(MapGenerationSetting::NORMAL);
	Engine::setDifficulty(Difficulty::NORMAL);

	Engine::start();
}