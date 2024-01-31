#pragma once
#include "EnemyBase.h"

class ThrowEnemy : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// /// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	ThrowEnemy(int handle, LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ThrowEnemy();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	void Update(Player& player)override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[�����G����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	void SearchForPlayer(VECTOR playerPos);

	/// <summary>
	/// �G���e��������
	/// </summary>
	/// <param name="shotManager">�e���Ǘ�����N���X�̃|�C���^</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <param name="height">�v���C���[�̍���</param>
	void Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height);

private:
	float innerProduct = 0.0f;						//���ς̌��ʂ�����

	bool isDetection_ = false;						//�v���C���[�����m������
	bool isThrow_ = false;							//�����郂�[�V��������

	VECTOR pushVec_ = {};							//�v���C���[���G�ɂԂ������Ƃ��ɉ����x�N�g��

	std::list<MV1_COLL_RESULT_POLY_DIM> hitDim_;
};

