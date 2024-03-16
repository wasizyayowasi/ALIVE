#pragma once
#include <cmath>
#include <algorithm>

/// <summary>
/// �Q�l�T�C�g
/// https://game-ui.net/?p=835
/// https://qiita.com/HnniTns/items/9e3799d3b414dccbbd7d
/// </summary>

namespace
{
	//�l
	constexpr float float_value_1 = 1.0f;

	//�l
	constexpr float float_value_2 = 2.0f;
}

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

	template <typename T>

	/// <summary>
	/// �n�߂ƍŌ�͊ɂ₩�ɒ��Ԃł͋}�ɂ���C�[�W���O�֐��@InOutCubic���������͂������
	/// </summary>
	/// <typeparam name="T">�e���v���[�g�^</typeparam>
	/// <param name="elapsedTime">�o�ߎ���</param>
	/// <param name="totalTime">���b�����Ĉړ����邩</param>
	/// <param name="maxValue">�ő�l</param>
	/// <param name="currentValue">���݂̒l</param>
	/// <returns>�l</returns>
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
