#pragma once
#include "CharacterBase.h"
#include <list>
#include <memory>
#include <unordered_map>

class Aster;
class ShotManager;

//�G�l�~�[�̃A�j���[�V�����^�C�v
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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	EnemyBase(int handle, Material materialType, LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~EnemyBase() {};

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player) override;

	/// <summary>
	/// �`��
	/// </summary>
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

	/// <summary>
	/// ���������ɃI�u�W�F�N�g�����邩
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>�I�u�W�F�N�g�����邩</returns>
	bool IsThereAnObject(VECTOR playerPos);

	/// <summary>
	/// �G���e��������
	/// </summary>
	/// <param name="shotManager">�e���Ǘ�����N���X�̃|�C���^</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="height">�v���C���[�̍���</param>
	void Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos, float height);

	/// <summary>
	/// ��]�s��Ɗg�k�s��̍���
	/// </summary>
	/// <returns>��]�s��Ɗg�k�s���������������</returns>
	MATRIX CombiningRotAndScallMat(VECTOR distance);

protected:

	bool isDetection_ = false;						//�v���C���[�����m������
	bool isThrow_ = false;							//�����郂�[�V��������

	VECTOR frontVec_ = {};							//�G�̐��ʃx�N�g��������
	VECTOR pushVec_ = {};							//�v���C���[���G�ɂԂ������Ƃ��ɉ����x�N�g��

	std::shared_ptr<Aster> Aster_;
};

