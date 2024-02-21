#pragma once
#include "GimmickBase.h"
#include <memory>
#include <vector>
#include <DxLib.h>
#include <unordered_map>

class Switch;
class Lever;
class Model;

class Elevator final : public GimmickBase
{
private:
	//�G���x�[�^�[�̃A�j���^�C�v
	enum class ElevatorAnimType
	{
		openIdle,
		closeIdle,
		open,
		close,
		max,
	};

	//�G���x�[�^�[�̏��
	enum class ElevatorState
	{
		upper,
		lower,
	};

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="handle">���f���n���h��</param>
	/// <param name="materialType">�}�e���A���̃^�C�v</param>
	/// <param name="objInfo">�z�u�f�[�^</param>
	Elevator(const int handle, const Material materialType, const LoadObjectInfo objInfo);
	
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
	virtual const std::shared_ptr<Model>& AddCollModel()const override;

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

	float moveVecY_ = 0.0f;								//Y���̈ړ����F�N�g��
	float elapsedTime_ = 0.0f;							//�o�ߎ���

	bool isDeparture_ = false;							//�G���x�[�^�[���o�����Ă��邩�ǂ���
	bool isPlaySound_ = false;							//�T�E���h���Đ�����
	bool isOnSwitch_ = false;							//�X�C�b�`�������Ă��邩

	VECTOR targetPos_ = {};								//���݌������Ă���|�W�V����

	ElevatorState state_ = {};							//�G���x�[�^�[�̏��

	std::unordered_map<ElevatorState, VECTOR> stopPos_;	//�X�g�b�v�|�W�V����

	std::shared_ptr<Switch> switch_;					//�X�C�b�`�N���X�̃X�}�[�g�|�C���^

	std::vector<std::shared_ptr<Lever>> levers_;		//���o�[�N���X�̃X�}�[�g�|�C���^��Vector�z��
};

