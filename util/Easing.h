#pragma once

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
	/// <returns></returns>
	T InOutCubic(T elapsedTime, T totalTime,T maxValue,T currentValue);
	
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

}
