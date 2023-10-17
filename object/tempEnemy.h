#pragma once
#include "EnemyBase.h"

class tempEnemy : public EnemyBase
{
public:
	tempEnemy(const char* const filename, LoadObjectInfo objInfo);
	tempEnemy(int handle, LoadObjectInfo objInfo);
	virtual ~tempEnemy();

	//�X�V
	void Update(Player& player) override;
	//�`��
	void Draw();

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

