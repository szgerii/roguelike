#pragma once

namespace CR {
	class GameObject {
	private:
		int x, y;
		char skin;
	public:
		GameObject(char skin, int x, int y)
			:skin(skin), x(x), y(y) {}

		GameObject(const GameObject* go);

		virtual char getSkin() const;
		virtual int getX() const;
		virtual int getY() const;

		virtual void setX(int value);
		virtual void setY(int value);
	};
}