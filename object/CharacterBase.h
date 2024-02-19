#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	CharacterBase(const int handle, const Material materialType, const LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CharacterBase() {};
protected:

	int animNo_ = 0;				//�A�j���[�V�����ԍ�

	bool isAnimLoop_ = false;		//�A�j���[�V���������[�v�Đ������邩

};

