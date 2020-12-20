#pragma once
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008
#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080
#define COMMON_LVB_REVERSE_VIDEO 0x4000
#define COMMON_LVB_UNDERSCORE 0x8000

namespace CR {
	class GameObject {
	private:
		char skin;
		unsigned short skinColor;
		int x, y;
	public:
		GameObject(char skin, unsigned short skinColor, int x, int y)
			:skin(skin), skinColor(skinColor), x(x), y(y) {}

		virtual char getSkin() const;
		virtual unsigned short getSkinColor() const;
		virtual void tick();
		virtual int getX() const;
		virtual int getY() const;

		virtual void setX(int value);
		virtual void setY(int value);
	};
}