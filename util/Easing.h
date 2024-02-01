#pragma once
#include <cmath>
#include <algorithm>

namespace Easing {

	template <typename T>
	/// <summary>
	/// �n�߂ƍŌ�͊ɂ₩�ɒ��Ԃł͋}�ɂ���C�[�W���O�֐�
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="elapsedTime">�o�ߎ���</param>
	/// <param name="totalTime">���b�����Ĉړ����邩</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	/// <returns>�l</returns>
	T InOutCubic(T elapsedTime, T totalTime,T maxValue,T currentValue);
	
	template <typename T>
	/// <summary>
	/// �n�߂͋}�ōŌ�ɂȂ�ɂ�Ċɂ₩�ɂȂ�
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="elapsedTime">�o�ߎ���</param>
	/// <param name="totalTime">���b�����Ĉړ����邩</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	/// <returns>�l</returns>
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
