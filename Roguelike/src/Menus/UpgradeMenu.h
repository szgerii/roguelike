#pragma once
#include <cmath>
#include "Menus/Menu.h"
#include "Menus/UltimateUpgradeMenu.h"
#include "Entity/Player.h"
#include "Engine/Game.h"

namespace CR::Menus {
	template <int screenWidth, int screenHeight>
	class UpgradeMenu : public Menu<screenWidth, screenHeight> {
	protected:
		bool skip;
		char price[5];

	public:
		UpgradeMenu(HANDLE screenBuffer, HANDLE inputBuffer)
			: Menu<screenWidth, screenHeight>(screenBuffer, inputBuffer) {
			bool skip = true;
			for (char i = 0; i < 5; i++) {
				if (canUpgrade(i)) {
					skip = false;
					break;
				}
			}
			this->skip = skip;

			if (skip)
				return;

			Entities::Player* player = (Entities::Player*)Engine::getPlayer();

			this->title = "U P G R A D E S";

			std::string next;
			
			if (player->getMaxHealthProp().canUpgrade())
				next = " (NEXT: " + std::to_string((int)round(player->getMaxHealthProp().getNextValue())) + ")";
			else if (player->getMaxHealthProp().canUltimateUpgrade()) {
				next = " (ULTIMATE AVAILABLE: " + std::to_string((int)round(player->getMaxHealthProp().getUltimate())) + ")";
				int tIndex = 0;
				do {
					price[tIndex] = getRandomNumberBetween(0, 4);
				} while (price[tIndex] == tIndex || !canUpgrade(price[tIndex]));
			} else
				next = " (MAX LEVEL)";
			this->options.push_back("Max Health: " + std::to_string((int)round(player->getMaxHealth())) + next);
			
			if (player->getMeleeDamageProp().canUpgrade())
				next = " (NEXT: " + std::to_string((int)round(player->getMeleeDamageProp().getNextValue())) + ")";
			else if (player->getMeleeDamageProp().canUltimateUpgrade()) {
				next = " (ULTIMATE AVAILABLE: " + std::to_string((int)round(player->getMeleeDamageProp().getUltimate())) + ")";
				int tIndex = 1;
				do {
					price[tIndex] = getRandomNumberBetween(0, 4);
				} while (price[tIndex] == tIndex || !canUpgrade(price[tIndex]));
			} else
				next = " (MAX LEVEL)";
			this->options.push_back("Melee Damage: " + std::to_string((int)round(player->getMeleeDamageProp().getValue())) + next);

			if (player->getWeaponDamageMultiplierProp().canUpgrade())
				next = " (NEXT: " + std::to_string(player->getWeaponDamageMultiplierProp().getNextValue()).substr(0, 4) + ")";
			else if (player->getWeaponDamageMultiplierProp().canUltimateUpgrade()) {
				next = " (ULTIMATE AVAILABLE: " + std::to_string(player->getWeaponDamageMultiplierProp().getUltimate()).substr(0, 4) + ")";
				int tIndex = 2;
				do {
					price[tIndex] = getRandomNumberBetween(0, 4);
				} while (price[tIndex] == tIndex || !canUpgrade(price[tIndex]));
			} else
				next = " (MAX LEVEL)";
			this->options.push_back("Weapon Damage Multiplier: " + std::to_string(player->getWeaponDamageMultiplierProp().getValue()).substr(0, 4) + next);
			
			if (player->getDamageReductionProp().canUpgrade())
				next = " (NEXT: " + std::to_string(player->getDamageReductionProp().getNextValue()).substr(0, 4) + ")";
			else if (player->getDamageReductionProp().canUltimateUpgrade()) {
				next = " (ULTIMATE AVAILABLE: " + std::to_string(player->getDamageReductionProp().getUltimate()).substr(0, 4) + ")";
				int tIndex = 3;
				do {
					price[tIndex] = getRandomNumberBetween(0, 4);
				} while (price[tIndex] == tIndex || !canUpgrade(price[tIndex]));
			} else
				next = " (MAX LEVEL)";
			this->options.push_back("Damage Reduction: " + std::to_string(player->getDamageReductionProp().getValue()).substr(0, 4) + next);
			
			if (player->getAmmoPickupMultiplierProp().canUpgrade())
				next = " (NEXT: " + std::to_string(player->getAmmoPickupMultiplierProp().getNextValue()).substr(0, 4) + ")";
			else if (player->getAmmoPickupMultiplierProp().canUltimateUpgrade()) {
				next = " (ULTIMATE AVAILABLE: " + std::to_string(player->getAmmoPickupMultiplierProp().getUltimate()).substr(0, 4) + ")";
				int tIndex = 4;
				do {
					price[tIndex] = getRandomNumberBetween(0, 4);
				} while (price[tIndex] == tIndex || !canUpgrade(price[tIndex]));
			} else
				next = " (MAX LEVEL)";
			this->options.push_back("Ammo Pickup Multiplier: " + std::to_string(player->getAmmoPickupMultiplierProp().getValue()).substr(0, 4) + next);

			this->calculateLongestOptionLength();
		}

		enum Results : char {
			MAX_HEALTH, MELEE_DAMAGE, WEAPON_DAMAGE, DAMAGE_REDUCTION, AMMO_PICKUP, CANCEL
		};

		virtual int show() {
			if (skip)
				return Results::CANCEL;

			return Menu<screenWidth, screenHeight>::show();
		}

		virtual int selectOption(std::string option) {
			Entities::Player* player = (Entities::Player*)Engine::getPlayer();

			switch (this->selectedIndex) {
			case 0:
				if (!player->getMaxHealthProp().canUpgrade() && !player->getMaxHealthProp().canUltimateUpgrade())
					return -1;
				else if (player->getMaxHealthProp().canUltimateUpgrade()) {
					Menus::UltimateUpgradeMenu<screenWidth, screenHeight> uum(this->screenBuffer, this->inputBuffer, 0, price[0]);
					if (uum.show() == uum.Results::YES) {
						downgrade(price[0]);
						return Results::MAX_HEALTH;
					} else
						return -1;
				} else
					return Results::MAX_HEALTH;

			case 1:
				if (!player->getMeleeDamageProp().canUpgrade() && !player->getMeleeDamageProp().canUltimateUpgrade())
					return -1;
				else if (player->getMeleeDamageProp().canUltimateUpgrade()) {
					Menus::UltimateUpgradeMenu<screenWidth, screenHeight> uum(this->screenBuffer, this->inputBuffer, 1, price[1]);
					if (uum.show() == uum.Results::YES) {
						downgrade(price[1]);
						return Results::MELEE_DAMAGE;
					} else
						return -1;
				} else
					return Results::MELEE_DAMAGE;

			case 2:
				if (!player->getWeaponDamageMultiplierProp().canUpgrade() && !player->getWeaponDamageMultiplierProp().canUltimateUpgrade())
					return -1;
				else if (player->getWeaponDamageMultiplierProp().canUltimateUpgrade()) {
					Menus::UltimateUpgradeMenu<screenWidth, screenHeight> uum(this->screenBuffer, this->inputBuffer, 2, price[2]);
					if (uum.show() == uum.Results::YES) {
						downgrade(price[2]);
						return Results::WEAPON_DAMAGE;
					} else
						return -1;
				} else
					return Results::WEAPON_DAMAGE;

			case 3:
				if (!player->getDamageReductionProp().canUpgrade() && !player->getDamageReductionProp().canUltimateUpgrade())
					return -1;
				else if (player->getDamageReductionProp().canUltimateUpgrade()) {
					Menus::UltimateUpgradeMenu<screenWidth, screenHeight> uum(this->screenBuffer, this->inputBuffer, 3, price[3]);
					if (uum.show() == uum.Results::YES) {
						downgrade(price[3]);
						return Results::DAMAGE_REDUCTION;
					} else
						return -1;
				} else
					return Results::DAMAGE_REDUCTION;

			case 4:
				if (!player->getAmmoPickupMultiplierProp().canUpgrade() && !player->getAmmoPickupMultiplierProp().canUltimateUpgrade())
					return -1;
				else if (player->getAmmoPickupMultiplierProp().canUltimateUpgrade()) {
					Menus::UltimateUpgradeMenu<screenWidth, screenHeight> uum(this->screenBuffer, this->inputBuffer, 4, price[4]);
					if (uum.show() == uum.Results::YES) {
						downgrade(price[4]);
						return Results::AMMO_PICKUP;
					} else
						return -1;
				} else
					return Results::AMMO_PICKUP;

			default:
				break;
			}

			return -1;
		}

		virtual void downgrade(char target) {
			Entities::Player* player = (Entities::Player*)Engine::getPlayer();
			
			switch (target) {
			case 0:
				player->getMaxHealthProp().downgrade(player->getMaxHealthProp().getStep());
				player->heal(1.0f);
				break;

			case 1:
				player->getMeleeDamageProp().downgrade(player->getMeleeDamageProp().getStep());
				break;

			case 2:
				player->downgradeWeaponDamage();
				break;

			case 3:
				player->getDamageReductionProp().downgrade(player->getDamageReductionProp().getStep());
				break;

			case 4:
				player->downgradePickupModifier();
				break;
			}
		}

		virtual bool canUpgrade(char target) {
			Entities::Player* player = (Entities::Player*)Engine::getPlayer();

			switch (target) {
			case 0:
				return player->getMaxHealthProp().canUpgrade() || player->getMaxHealthProp().canUltimateUpgrade();

			case 1:
				return player->getMeleeDamageProp().canUpgrade() || player->getMeleeDamageProp().canUltimateUpgrade();

			case 2:
				return player->getWeaponDamageMultiplierProp().canUpgrade() || player->getWeaponDamageMultiplierProp().canUltimateUpgrade();

			case 3:
				return player->getDamageReductionProp().canUpgrade() || player->getDamageReductionProp().canUltimateUpgrade();

			case 4:
				return player->getAmmoPickupMultiplierProp().canUpgrade() || player->getAmmoPickupMultiplierProp().canUltimateUpgrade();

			default:
				return false;
			}
		}
	};
}
