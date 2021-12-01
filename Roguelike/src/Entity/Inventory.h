#pragma once
#include <string>
#include <cmath>
#include <stdexcept>
#include "Engine/GameObject.h"
#include "Weapon/Weapon.h"
#include "Object/Tiles/WeaponTile.h"

namespace CR {
	template <int size>
	class Inventory {
	protected:
		GameObject* owner;
		int statIndex, itemStatIndex, selectedIndex = 0, usedSpace = 0;
		Weapons::Weapon* storage[size];

	public:
		Inventory(GameObject* owner, int statIndex = -1, int itemStatIndex = -1)
			: owner(owner), statIndex(statIndex), itemStatIndex(itemStatIndex) {
			for (int i = 0; i < size; i++)
				storage[i] = nullptr;
			
			setIndex(0);
		}

		virtual void setIndex(int index) {
			if (index < 0 || index >= size)
				throw std::invalid_argument("The index was outside the bounds of the inventory array");

			if (itemStatIndex != -1) {
				Engine::modifyGUIText(itemStatIndex, "");

				if (storage[selectedIndex] != nullptr)
					storage[selectedIndex]->setStatIndex(-1);
				if (storage[index] != nullptr)
					storage[index]->setStatIndex(itemStatIndex);
			}

			selectedIndex = index;
			drawStat();
		}

		virtual void nextItem() {
			int index = selectedIndex + 1;

			if (index >= size)
				index = 0;

			setIndex(index);
		}

		virtual void previousItem() {
			int index = selectedIndex - 1;

			if (index < 0)
				index = size - 1;

			setIndex(index);
		}

		virtual bool pickUpItem(Weapons::Weapon* item) {
			if (usedSpace == size)
				return false;

			int i = 0;
			while (storage[i] != nullptr)
				i++;

			item->changeOwner(owner);
			storage[i] = item;
			usedSpace++;
			setIndex(selectedIndex);
			drawStat();
			return true;
		}

		virtual void dropItem() {
			dropItem(selectedIndex);
		}

		virtual void dropItem(int index) {
			if (usedSpace == 0 || storage[index] == nullptr)
				return;

			if (index == selectedIndex && itemStatIndex != -1)
				Engine::modifyGUIText(itemStatIndex, "");

			storage[index]->changeOwner(nullptr);

			Objects::WeaponTile* wTile;
			if ((int)round(owner->getX()) != 1)
				wTile = new Objects::WeaponTile({ (int)round(owner->getX()) - 1, (int)round(owner->getY()) }, storage[index]);
			else
				wTile = new Objects::WeaponTile({ (int)round(owner->getX()) + 1, (int)round(owner->getY()) }, storage[index]);
			Engine::addGameObject(wTile);

			storage[index] = nullptr;

			usedSpace--;
			drawStat();
		}

		virtual void drawStat() {
			if (statIndex == -1)
				return;

			std::string output;

			for (int i = 0; i < size; i++) {
				if (i == selectedIndex)
					if (storage[i] != nullptr)
						output += "([X]) ";
					else		
						output += "([ ]) ";
				else if (storage[i] != nullptr)
					output += " [X]  ";
				else
					output += " [ ]  ";
			}

			output.erase(output.length() - 1);

			Engine::modifyGUIText(statIndex, output);
		}

		virtual Weapons::Weapon* getCurrentItem() {
			return storage[selectedIndex];
		}
	};
}