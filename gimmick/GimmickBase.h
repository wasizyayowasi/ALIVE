#pragma once
#include "../object/OrnamentBase.h"

class GimmickBase : public OrnamentBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	GimmickBase(const int handle, const Material materialType, const LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~GimmickBase();
};

