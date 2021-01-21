#include "Wave.h"
#include <cmath>
#include "Engine/Game.h"
#include "Entity/Enemies/Pavaka.h"
#include "Entity/Enemies/Turret.h"
#include "Entity/Enemies/ShootyBoi.h"
#include "Object/Tiles/AmmoTile.h"
#include "Object/Tiles/HealthTile.h"
#include "Object/Tiles/EndTile.h"
#include "Utils.h"

namespace CR {
	Wave::Wave(int enemyCount, int ammoTileCount, int healthTileCount, unsigned char* room, Weapons::Weapon* weapon) {
		int mapWidth = Engine::getMapWidth(),
			mapHeight = Engine::getMapHeight();
		
		// Enemies
		int turretCount = 0, pavakaCount = 0, shootyBoiCount = 0, maxUnique = (int)round(enemyCount * 0.4);
		maxUnique = enemyCount;
		for (int i = 0; i < enemyCount; i++) {
			int rand = getRandomNumberBetween(1, 3);
			
			int x, y;
			do {
				x = getRandomNumberBetween((int)round(mapWidth * 0.25), (int)round(mapWidth * 0.75));
				y = getRandomNumberBetween(1, mapHeight - 3);
			} while (room[y * mapWidth + x] != 0 && room[y * mapWidth + x] != 1);

			switch (rand) {
			case 1:
				if (turretCount + 1 > maxUnique) {
					i--;
					continue;
				}

				gameObjects.push_back(new Entities::Turret({ (float)x, (float)y }));
				turretCount++;
				break;

			case 2:
				if (pavakaCount + 1 > maxUnique) {
					i--;
					continue;
				}

				gameObjects.push_back(new Entities::Pavaka({ (float)x, (float)y }));
				pavakaCount++;
				break;

			case 3:
				if (shootyBoiCount + 1 > maxUnique) {
					i--;
					continue;
				}

				gameObjects.push_back(new Entities::ShootyBoi({ (float)x, (float)y }));
				shootyBoiCount++;
				break;
			}
			room[y * mapWidth + x] = 255;
			gameObjects[gameObjects.size() - 1]->setFreeze(true);
		}

		int x, y;

		// Ammo tiles
		for (int i = 0; i < ammoTileCount; i++) {
			int x, y;
			do {
				x = getRandomNumberBetween((int)round(mapWidth * 0.25), (int)round(mapWidth * 0.75));
				y = getRandomNumberBetween(1, mapHeight - 3);
			} while (room[y * mapWidth + x] != 0 && room[y * mapWidth + x] != 1);

			room[y * mapWidth + x] = 255;
			gameObjects.push_back(new Objects::AmmoTile({ x, y }));
		}

		// Health tiles
		for (int i = 0; i < healthTileCount; i++) {
			int x, y;
			do {
				x = getRandomNumberBetween(0, mapWidth - 2);
				y = getRandomNumberBetween(1, mapHeight - 3);
			} while (room[y * mapWidth + x] != 0 && room[y * mapWidth + x] != 1);

			room[y * mapWidth + x] = 255;
			gameObjects.push_back(new Objects::HealthTile({ x, y }, 13));
		}

		// Weapon Tile
		if (weapon != nullptr) {
			do {
				x = getRandomNumberBetween(0, mapWidth - 2);
				y = getRandomNumberBetween(1, mapHeight - 3);
			} while (room[y * mapWidth + x] != 0 && room[y * mapWidth + x] != 1);

			room[y * mapWidth + x] = 255;
			gameObjects.push_back(new Objects::WeaponTile({ x, y }, weapon));
		}
	}

	void Wave::activate() {
		for (auto& obj : gameObjects)
			Engine::addGameObject(obj);
	}
}