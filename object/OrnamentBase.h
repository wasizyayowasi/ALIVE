#pragma once
#include "ObjectBase.h"

class OrnamentBase : public ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	OrnamentBase(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~OrnamentBase();
};

