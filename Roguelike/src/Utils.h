#pragma once
#include <Windows.h>
#include <chrono>

#ifndef FG_WHITE
#define FG_WHITE FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN
#endif

#ifndef BG_WHITE
#define BG_WHITE BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN
#endif

namespace CR {
	template <typename T>
	struct Vector2 {
	public:
		T x, y;

		Vector2(T x, T y)
			: x(x), y(y) {}
	};

	template <typename precision = std::chrono::milliseconds>
	long long getCurrentTime() {
		return std::chrono::time_point_cast<precision>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();
	}
}