#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>

class Aster;

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
	void Draw()override;

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

	/// <summary>
	/// �v���C���[��˂���΂�
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void ThrustAway(Player& player);

	/// <summary>
	/// �o�H�T��
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void RoutingUpdate(Player& player);

	/*/// <summary>
	/// �I�u�W�F�N�g��������Ȃ���ǐՂ���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void AvoidAndTrackObjectsUpdate(VECTOR playerPos);

	/// <summary>
	/// �I�u�W�F�N�g�Ƃ̐��ʏՓ˂��`�F�b�N����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void CheckHeadOnCollision();*/

private:

	float distance_ = 0.0f;				//�G����v���C���[�̋���
	float innerProduct = 0.0f;			//���ς̌��ʂ�����

	bool isMove_ = false;				//�ړ����t���O
	bool isHit_ = false;

	VECTOR frontVec_ = { 0.0f,0.0f ,-1.0f };		//�G�̐��ʃx�N�g��������
	VECTOR playerpos_ = { 0.0f,0.0f ,-1.0f };		//�G�̐��ʃx�N�g��������

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

	MV1_COLL_RESULT_POLY_DIM temp = {};

	std::shared_ptr<Aster> Aster_;

};

