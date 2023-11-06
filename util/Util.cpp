#include "Util.h"
#include "InputState.h"
#include <algorithm>

template<typename T>
inline Util<T>::Util()
{
}

template<typename T>
inline Util<T>::~Util()
{
}

template<typename T>
inline void Util<T>::SelectValueUpdate(const InputState& input,T up, T down, T& selectValue, const T minValue, const T maxValue, const T addValue)
{
	if (input.IsTriggered(up)) {
		selectValue = (std::max)(selectValue - addValue, minValue);
	}
	if (input.IsTriggered(down)) {
		selectValue = (std::min)(selectValue + addValue, maxValue);
	}
}