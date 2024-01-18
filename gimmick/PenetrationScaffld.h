#pragma once
#include "GimmickBase.h"

class PenetrationScaffld : public GimmickBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	PenetrationScaffld(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~PenetrationScaffld();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:

	int materialNum_ = 0;				//�}�e���A���̐�

	float alphaValue_ = 1.0f;			//�A���t�@�l

	bool alphaValueIncrease_ = false;	//�A���t�@�l�̑���
};

