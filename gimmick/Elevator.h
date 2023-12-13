#pragma once
#include "GimmickBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Switch;
class Lever;
class Model;

class Elevator : public GimmickBase
{
private:

	enum class ElevatorAnimType {
		openIdle,
		closeIdle,
		open,
		close,
		max,
	};

public:
	//�R���X�g���N�^
	Elevator(int handle, LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~Elevator();

	//�X�V
	void Update(Player& player, const InputState& input)override;
	//�`��
	void Draw();

	//�Փ˔�����s�����f����ǉ�����
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

	bool isDeparture_ = false;						//�G���x�[�^�[���o�����Ă��邩�ǂ���

	VECTOR targetPos_ = {};							//���݌������Ă���|�W�V����

	std::shared_ptr<Switch> switch_;				//�X�C�b�`�N���X�̃X�}�[�g�|�C���^

	std::vector<std::shared_ptr<Lever>> levers_;	//���o�[�N���X�̃X�}�[�g�|�C���^��Vector�z��
};

