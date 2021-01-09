#pragma once

namespace CR {
	template <typename T>
	struct Vector2 {
	public:
		T x, y;

		Vector2(T x, T y)
			: x(x), y(y) {}
	};
}