#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

enum class EnemyAnimType {
	Idle,
	Walk,
	Run,
	Death,
	Throw,
	max,
};

class EnemyBase : public CharacterBase
{
public:
	//�쐬�ς݃��f���Ń��f���N���X���C���X�^���X������R���X�g���N�^
	EnemyBase(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~EnemyBase() {};

	//�X�V
	void Update(Player& player, const InputState& input) override;
	//�`��
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

	/// <summary>
	/// �G���e��������
	/// </summary>
	/// <param name="shotManager">�e���Ǘ�����N���X�̃|�C���^</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="height">�v���C���[�̍���</param>
	void Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos, float height);

private:

	float distanceSize_ = 0.0f;						//�G����v���C���[�̋���
	float innerProduct = 0.0f;						//���ς̌��ʂ�����

	bool isMove_ = false;							//�ړ����t���O
	bool isDetection_ = false;						//�v���C���[�����m������
	bool isThrow_ = false;							//�����郂�[�V��������

	VECTOR frontVec_ = {};							//�G�̐��ʃx�N�g��������
	VECTOR pushVec_ = {};							//�v���C���[���G�ɂԂ������Ƃ��ɉ����x�N�g��

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;

	std::shared_ptr<Aster> Aster_;


};

