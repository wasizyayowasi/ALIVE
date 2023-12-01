#include "Util.h"
#include <algorithm>
#include <stdio.h>

namespace MathUtil 
{
	//��_�Ԃ̋����̃T�C�Y���擾����
	float GetSizeOfDistanceTwoPoints(VECTOR pos, VECTOR targetPos)
	{
		VECTOR distance = {};
		distance = VSub(pos, targetPos);

		return VSize(distance);
	}

}

namespace StrUtil
{
	//�����񂩂琔�l���擾����
	int StrUtil::GetNumberFromString(std::string name, std::string sign)
	{
		//������̐����擾
		int strSize = name.size();

		//������̐���0�ȉ���������0��Ԃ�
		if (strSize <= 0) {
			return 0;
		}

		//�����񂩂������������ԍ����擾����
		int searchStrNum = name.find(sign);

		//��L�Ŏ擾�����ԍ�����������̕�����̖����܂ł��擾
		auto str = name.substr(searchStrNum + 1, strSize);

		//������𐔒l�ɕϊ�����
		int num = atoi(str.c_str());

		return num;
	}

	std::string GetConcatenateNumAndStrings(std::string name, std::string sign, int num)
	{
		//���l�𕶎���ɂ���
		std::string value = std::to_string(num);
		//name�ɋL��(sign)�ƕ�����ɕϊ�����value(num)��A��
		name = name + sign + value;

		return name;
	}

	std::string GetStringWithPartsAfterTheSymbolDeleted(std::string name, std::string sign)
	{
		//�������ۊ�
		std::string str = name;

		if (name.find(sign) != -1) {
			str = str.erase(name.find(sign));
		}

		return str;
	}

	std::string GetStringAfterSign(std::string name, std::string sign)
	{
		//������̃T�C�Y���擾
		int size = name.size();

		//�u.�v��������̉��Ԗڂ����擾����
		int dotNum = name.find(sign);

		//�u.�v�ȍ~����Ō�܂ł̕�������擾����
		std::string str = name.substr(dotNum + 1, size);

		return str;
	}

}


