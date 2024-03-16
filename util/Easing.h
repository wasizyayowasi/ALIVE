#pragma once
#include <cmath>
#include <algorithm>

/// <summary>
/// 参考サイト
/// https://game-ui.net/?p=835
/// https://qiita.com/HnniTns/items/9e3799d3b414dccbbd7d
/// </summary>

namespace
{
	//値
	constexpr float float_value_1 = 1.0f;

	//値
	constexpr float float_value_2 = 2.0f;
}

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

	template <typename T>

	/// <summary>
	/// 始めと最後は緩やかに中間では急にするイージング関数　InOutCubicよりも加速はゆっくり
	/// </summary>
	/// <typeparam name="T">テンプレート型</typeparam>
	/// <param name="elapsedTime">経過時間</param>
	/// <param name="totalTime">何秒かけて移動するか</param>
	/// <param name="maxValue">最大値</param>
	/// <param name="currentValue">現在の値</param>
	/// <returns>値</returns>
	T InOutSine(T elapsedTime, T totalTime, T maxValue, T currentValue);
}

namespace Easing
{
	template <typename T = float>
	T InOutCubic(T elapsedTime, T totalTime, T maxValue, T currentValue) 
	{
		maxValue -= currentValue;
		elapsedTime /= totalTime;

		if (elapsedTime / float_value_2 < float_value_1)
		{
			return maxValue / float_value_2 * elapsedTime * elapsedTime * elapsedTime + currentValue;
		}

		elapsedTime -= maxValue;

		return maxValue / float_value_2 * (elapsedTime * elapsedTime * elapsedTime + float_value_2) + currentValue;
	}

	template <typename T = float>
	T OutCirc(T elapsedTime, T totalTime, T maxValue, T currentValue)
	{
		maxValue -= currentValue;
		elapsedTime /= (totalTime - float_value_1);

		elapsedTime = (std::max)((std::min)(elapsedTime, float_value_1), -float_value_1);

		return maxValue * std::sqrt(float_value_1 - elapsedTime * elapsedTime) + currentValue;
	}

	template <typename T = float>
	T InOutSine(T elapsedTime, T totalTime, T maxValue, T currentValue)
	{
		maxValue -= currentValue;

		return -maxValue / float_value_2 * (std::cos(elapsedTime * DX_PI_F / totalTime) - float_value_1) + currentValue;
	}
}
