#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>
#include <unordered_map>

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
	bool SearchForPlayer(VECTOR playerPos);

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

	/// <summary>
	/// ���������ɃI�u�W�F�N�g�����邩
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�I�u�W�F�N�g�����邩</returns>
	bool IsThereAnObject(VECTOR playerPos);

	/// <summary>
	/// �������͈͓���������
	/// </summary>
	/// <returns></returns>
	bool DistanceIsWithinRange();

private:

	int time = 60;

	float distance_ = 0.0f;				//�G����v���C���[�̋���
	float innerProduct = 0.0f;			//���ς̌��ʂ�����

	bool isMove_ = false;				//�ړ����t���O
	bool isHit_ = false;

	VECTOR frontVec_ = { 0.0f,0.0f ,-1.0f };		//�G�̐��ʃx�N�g��������

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

	MV1_COLL_RESULT_POLY_DIM temp = {};

	std::shared_ptr<Aster> Aster_;

	std::unordered_map<int, VECTOR> debug_;

};

