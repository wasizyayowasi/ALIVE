#pragma once
#include "DxLib.h"
#include "CharacterBase.h"
#include "util/PlayerData.h"
#include <list>
#include <unordered_map>
#include <memory>

class InputState;
class Model;
class CheckCollisionModel;
class GimmickBase;

struct JumpInfo {
	float jumpVec;		//�W�����v�x�N�g��
	bool isJump;		//�W�����v�����ǂ���
};

struct PlayerStatus {
	JumpInfo jump;
	VECTOR pos;
	VECTOR rot;
	VECTOR moveVec;
	int animNo;
	float height;
	bool isAnimLoop;
	bool isTransit;
};

class Player
{
public:

	Player(const char* const filename);
	Player(int handle);
	virtual ~Player();

	void Init();

	/// <summary>
	/// �v���C���[�̍X�V���s��
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	/// <param name="models">�Փ˔�����s�����f����vector�^�̔z��</param>
	void Update(const InputState& input);

	/// <summary>
	/// �v���C���[�֘A�̕`��
	/// </summary>
	void Draw();

	VECTOR GetRot();

	/// <summary>
	/// ���񂾉񐔂��擾����
	/// </summary>
	int getDeathCount() { return deathCount_; }

	/// <summary>
	/// �O������̃|�W�V�������󂯎��
	/// </summary>
	/// <param name="pos">�|�W�V�������</param>
	void SetPos(VECTOR pos);

	/// <summary>
	/// �O������̃W�����v�����󂯎��
	/// </summary>
	/// <param name="isJump">�W�����v�t���O</param>
	/// <param name="jumpVec">�W�����v�x�N�g��</param>
	void SetJumpInfo(bool isJump, float jumpVec);

	/// <summary>
	/// �o��邩�̏����󂯎��
	/// </summary>
	/// <returns>�o��邩�̃t���O</returns>
	void SetClim(bool isClim) { isClim_ = isClim; }

	/// <summary>
	/// �����^�Ԏ����o����t���O�Ǝ����^�ԃ��f���̃|�C���^���󂯎��
	/// </summary>
	void SetCarryInfo(bool isCarry, std::shared_ptr<Model> model);

	void SetSaveData(VECTOR pos);

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X���擾����
	/// </summary>
	PlayerStatus GetStatus() { return status_; }

	/// <summary>
	/// �����^�Ԏ��̂̃|�C���^�[���擾����
	/// </summary>
	std::shared_ptr<Model> GetDeadPersonModelPointer() {return deadPersonModelPointer_;}
		
private:
	//�ʏ�X�V
	void IdleUpdate(const InputState& input);

	//�ҋ@����
	void ChangeAnimIdle();

	//�ړ�����
	void MovingUpdate(const InputState& input);

	//�ړ�
	float Move(const InputState& input);

	//��]����
	void RotationUpdate();

	//�I�u�W�F�N�g��o��
	void ClimUpdate(const InputState& input);

	//�W�����v����
	void JumpUpdate(const InputState& input);

	//����W�����v����
	void RunningJumpUpdate(const InputState& input);

	//���S����
	void DeathUpdate(const InputState& input);

	//���̂̌㏈��
	void DeathPersonPostProsessing();

	//���l����
	void DeadPersonGenerater();

	//����
	void SitUpdate(const InputState& input);

	//�����Ă���r��
	void IdleToSitup(const InputState& input);

	//�����オ��
	void StandUpdate(const InputState& input);

	//�����^��
	void CarryObjectUpdate();

	//�ו������낷
	void DropOffObjectUpdate();

private:
	/// <summary>
	/// �v���C���[�̈ړ����x��ݒ肷��
	/// </summary>
	/// <returns>float�^�̈ړ����x</returns>
	float PlayerSpeed(bool pressedShift);

	/// <summary>
	/// �W�����v�̐ݒ���s��
	/// </summary>
	/// <param name="jumpPower">�W�����v��</param>
	void PlayerJump(float jumpPower);

	/// <summary>
	/// �w�肵��2�t���[���̒��S���W���Z�o����
	/// </summary>
	VECTOR FramPosition(const char* const LeftFramename, const char* const RightFramename);

	/// <summary>
	/// �A�j���[�V�����̕ύX���s��
	/// </summary>
	/// <param name="type">�A�j���[�V�����̃^�C�v</param>
	void ChangeAnimNo(AnimType type,bool isAnimLoop,int changeTime);

	/// <summary>
	/// �x���@����ʓx�@�ɕϊ����ĕԂ�
	/// </summary>
	/// <param name="rot">�p�x</param>
	VECTOR DegreesToRadians(VECTOR rot);

private:

	int deathCount_ = 0;					//���񂾉񐔂��L�^����

	float temp = 0;
	float tempGravity = 0.0f;

	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float tempAngle_ = 0.0f;

	bool isMoving_ = false;					//�ړ�����
	bool isSitting_ = false;				//�����Ă��邩
	bool isClim_ = false;
	bool isCanBeCarried_ = false;

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//���ԃ|�C���g

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//���̂̃|�W�V����

	std::shared_ptr<Model> deadPersonModelPointer_;				//�����^�Ԏ��̂̃��f���|�C���^
	std::shared_ptr<Model> PModel_;								//���f���N���X�̃|�C���^
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^

	std::unordered_map<AnimType, int> animType_;				

	void(Player::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^
	void(Player::* carryUpdateFunc_)();		//�����o�֐��|�C���^
};

