#pragma once
#include <Windows.h>

/*#define FLAG_MASK(x) (1 << x)

struct ControlKeyState {
	bool alt, ctrl, shift, numLock, capsLock, scrollLock;

	ControlKeyState(unsigned int cks) {
		alt = cks & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED) > 0;
		ctrl = cks & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED) > 0;
		shift = cks & SHIFT_PRESSED > 0;
		numLock = cks & NUMLOCK_ON > 0;
		capsLock = cks & CAPSLOCK_ON > 0;
		scrollLock = cks & SCROLLLOCK_ON > 0;
	}
};*/

namespace CR {
	class KeyEvent {
	public:
		bool keyDown;
		unsigned short repeatCount;
		unsigned short keyCode;
		char keyValue;

		KeyEvent(KEY_EVENT_RECORD e)
			: keyDown(e.bKeyDown), repeatCount(e.wRepeatCount), keyCode(e.wVirtualKeyCode), keyValue(e.uChar.AsciiChar) {}
	};
}