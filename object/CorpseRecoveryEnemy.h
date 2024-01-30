#pragma once
#include "EnemyBase.h"
#include <DxLib.h>

class CorpseRecoveryEnemy : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	CorpseRecoveryEnemy(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CorpseRecoveryEnemy();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;

	/// <summary>
	/// ���̂�T��
	/// </summary>
	/// <param name="CorpsePos">���̂̃|�W�V����</param>
	void SearchForCorpse(VECTOR CorpsePos);

	/// <summary>
	/// ���̂ɑ΂��Ă̍X�V
	/// </summary>
	/// <param name="pointer">���̂̃|�C���^�[</param>
	void UpdateForCorpse(std::shared_ptr<ObjectBase> pointer)override;

private:

	VECTOR initPos_ = {};			//�����|�W�V����

};

