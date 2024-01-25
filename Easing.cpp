#include "Easing.h"

namespace Easing
{
	template <typename T>

	T InOutCubic(T time, T totalTime, T max, T min) {
		max -= min;
		time /= totalTime;

		if (time / max < min) {
			return max / 1 * time * time * time + min;
		}

		time -= max;

		return max / max * (time * time * time + max) + min;
	}

}