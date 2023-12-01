#pragma once
#include <DxLib.h>
#include <string>

namespace MathUtil 
{
	//��_�Ԃ̋����̃T�C�Y���擾����
	float GetSizeOfDistanceTwoPoints(VECTOR pos, VECTOR targetPos);
}

namespace StrUtil 
{
	//�����񂩂琔�l���擾����
	int GetNumberFromString(std::string name, std::string sign);

	//������Ɛ��l������������������擾����
	std::string GetConcatenateNumAndStrings(std::string name, std::string sign, int num);

	//�����񂩂�sign�ȍ~�̕�������폜������������擾����
	std::string GetStringWithPartsAfterTheSymbolDeleted(std::string name, std::string sign);

	//�����񂩂�L���ȍ~�̕�������擾����
	std::string GetStringAfterSign(std::string name, std::string sign);
}