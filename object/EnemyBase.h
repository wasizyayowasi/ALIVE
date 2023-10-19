#pragma once
#include "CharacterBase.h"

class EnemyBase : public CharacterBase
{
public:
	//�t�@�C���p�X�Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(const char* fileName, LoadObjectInfo objInfo);
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~EnemyBase() {};

	void Update(Player& player) override;

	/// <summary>
	/// �v���C���[��ǐՂ���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void TrackingUpdate(VECTOR playerPos);

	/// <summary>
	/// �v���C���[�����G����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void SearchForPlayer(VECTOR playerPos);

private:

	float distance_ = 0.0f;				//�G����v���C���[�̋���
	float innerProduct = 0.0f;			//���ς̌��ʂ�����

	VECTOR frontVector_ = { 0.0f,0.0f ,-1.0f };		//�G�̐��ʃx�N�g��������

};

