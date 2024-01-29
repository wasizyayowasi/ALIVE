#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Lever;
class Model;

class Elevator final : public GimmickBase
{
private:
	//�G���x�[�^�[�̃A�j���^�C�v
	enum class ElevatorAnimType {
		openIdle,
		closeIdle,
		open,
		close,
		max,
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	Elevator(int handle, LoadObjectInfo objInfo);
	
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Elevator();

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
	/// �Փ˔�����s�����f���̒ǉ�
	/// </summary>
	/// <returns></returns>
	virtual std::shared_ptr<Model> AddCollModel();

private:

	/// <summary>
	/// �G���x�[�^�[���ړ�������
	/// </summary>
	void Move();

	/// <summary>
	/// �ړ���̃|�W�V�������擾����
	/// </summary>
	void TargetPosition();

private:

	float moveVecY_ = 0.0f;							//Y���̈ړ����F�N�g��
	float elapsedTime_ = 0.0f;

	bool isDeparture_ = false;						//�G���x�[�^�[���o�����Ă��邩�ǂ���

	VECTOR targetPos_ = {};							//���݌������Ă���|�W�V����

	std::shared_ptr<Switch> switch_;				//�X�C�b�`�N���X�̃X�}�[�g�|�C���^

	std::vector<std::shared_ptr<Lever>> levers_;	//���o�[�N���X�̃X�}�[�g�|�C���^��Vector�z��
};

