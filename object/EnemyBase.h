#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

class EnemyBase : public CharacterBase
{
public:
	//�t�@�C���p�X�Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(const char* fileName, LoadObjectInfo objInfo);
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~EnemyBase() {};

	//�X�V
	void Update(Player& player, const InputState& input) override;
	//�`��
	void Draw()override;

	//�Փ˔���
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

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

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

	void Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos, float height);

private:

	int time = 60;

	float distance_ = 0.0f;							//�G����v���C���[�̋���
	float innerProduct = 0.0f;						//���ς̌��ʂ�����

	bool isMove_ = false;							//�ړ����t���O

	VECTOR frontVec_ = {};							//�G�̐��ʃx�N�g��������
	VECTOR pushVec_ = {};
	VECTOR shotMoveVec_ = {};

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

	std::shared_ptr<Aster> Aster_;


};

