#pragma once

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
