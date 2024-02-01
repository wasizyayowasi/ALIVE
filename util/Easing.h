#pragma once
#include <cmath>
#include <algorithm>

namespace Easing {

	template <typename T>
	/// <summary>
	/// 始めと最後は緩やかに中間では急にするイージング関数
	/// </summary>
	/// <typeparam name="T">テンプレート型</typeparam>
	/// <param name="elapsedTime">経過時間</param>
	/// <param name="totalTime">何秒かけて移動するか</param>
	/// <param name="maxValue">最大値</param>
	/// <param name="currentValue">現在の値</param>
	/// <returns>値</returns>
	T InOutCubic(T elapsedTime, T totalTime,T maxValue,T currentValue);
	
	template <typename T>
	/// <summary>
	/// 始めは急で最後になるにつれて緩やかになる
	/// </summary>
	/// <typeparam name="T">テンプレート型</typeparam>
	/// <param name="elapsedTime">経過時間</param>
	/// <param name="totalTime">何秒かけて移動するか</param>
	/// <param name="maxValue">最大値</param>
	/// <param name="currentValue">現在の値</param>
	/// <returns>値</returns>
	T OutCirc(T elapsedTime, T totalTime, T maxValue, T currentValue);
}

namespace Easing
{
	template <typename T>
	T InOutCubic(T elapsedTime, T totalTime, T maxValue, T currentValue) {
		maxValue -= currentValue;
		elapsedTime /= totalTime;

		if (elapsedTime / 2.0f < 1.0f) {
			return maxValue / 2.0f * elapsedTime * elapsedTime * elapsedTime + currentValue;
		}

		elapsedTime -= maxValue;

		return maxValue / 2.0f * (elapsedTime * elapsedTime * elapsedTime + 2.0f) + currentValue;
	}

	template <typename T>
	T OutCirc(T elapsedTime, T totalTime, T maxValue, T currentValue) {
		maxValue -= currentValue;
		elapsedTime /= (totalTime - 1.0f);

		elapsedTime = (std::max)((std::min)(elapsedTime, 1.0f), -1.0f);

		return maxValue * std::sqrt(1.0f - elapsedTime * elapsedTime) + currentValue;
	}
}
