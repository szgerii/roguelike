#pragma once

namespace CR {
	template <typename T>
	class Property {
	protected:
		T value, step, limit, ultimate;
		bool increasing;

	public:
		Property(T value, T step, T limit, T ultimate, bool increasing=true)
			: value(value), step(step), limit(limit), ultimate(ultimate), increasing(increasing) { }

		void setValue(T newValue, bool noCheck=false) {
			value = newValue;

			if (!noCheck && ((increasing && value > limit) || (!increasing && value < limit)))
				value = limit;
		}

		bool canUpgrade() const {
			return (increasing && value < limit) || (!increasing && value > limit);
		}

		bool canUltimateUpgrade() const {
			return value == limit;
		}

		void upgrade(bool ultimateUpgrade = false) {
			if (ultimateUpgrade && value == limit)
				setValue(ultimate, true);
			else
				setValue(value + (increasing ? step : -step));
		}

		void downgrade(T amount) {
			value += increasing ? -amount : amount;
		}

		T getValue() const {
			return value;
		}

		T getStep() const {
			return step;
		}

		T getLimit() const {
			return limit;
		}

		T getUltimate() const {
			return ultimate;
		}

		T getNextValue() const {
			T result = value + (increasing ? step : -step);

			if ((increasing && result > limit) || (!increasing && result < limit))
				result = limit;

			return result;
		}

		bool isIncreasing() const {
			return increasing;
		}
	};
}