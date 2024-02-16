#pragma once
#include "CharacterBase.h"

class Model;

class Corpse final : public CharacterBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	Corpse(int handle, Material materialType, LoadObjectInfo objInfo, int animNo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Corpse();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
};
