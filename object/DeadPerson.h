#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson final : public CharacterBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~DeadPerson();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
private:

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

