#pragma once
#include "DxLib.h"
#include "CharacterBase.h"
#include "util/ObjAnimType.h"
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

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int anim_[static_cast<int>(AnimType::max)];
};

struct PlayerStatus {
	JumpInfo jump;
	VECTOR pos;
	VECTOR rot;
	VECTOR moveVec;
	int animNo;
	float height;
	bool isAnimLoop;
	bool isCarry;
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

	void SetClim(bool isClim) { isClim_ = isClim; }
	void SetCarryInfo(bool isCarry, std::shared_ptr<Model> model);

	void SetSaveData(VECTOR pos, int num, bool isContinue);
	int GetDeathNum() {return deathCount_;	}

	PlayerStatus GetStatus() { return status_; }

	std::shared_ptr<Model> GetTempCustodyPointer() {return temporaryCustodyPointer_;}
		
private:
	//�ʏ�X�V
	void IdleUpdate(const InputState& input);

	//�ҋ@����
	void ChangeAnimIdle();

	//�ړ�����
	void MovingUpdate(const InputState& input);

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
	void CarryObjectUpdate(const InputState& input);

private:
	/// <summary>
	/// �v���C���[�̈ړ����x��ݒ肷��
	/// </summary>
	/// <returns>float�^�̈ړ����x</returns>
	float PlayerSpeed(bool pressedShift);


	VECTOR FramPosition(const char* const LeftFramename, const char* const RightFramename);

private:

	float temp = 0;
	float tempGravity = 0.0f;

	int deathCount_ = 0;

	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float tempAngle_ = 0.0f;

	bool isMoving_ = false;					//�ړ�����
	bool isSitting_ = false;				//�����Ă��邩
	bool isClim_ = false;
	bool isContinue_ = false;				//���ꂾ�߂��Ǝv��

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//���ԃ|�C���g

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//���̂̃|�W�V����

	std::shared_ptr<Model> temporaryCustodyPointer_;
	std::shared_ptr<Model> PModel_;								//���f���N���X�̃|�C���^
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^

	std::unordered_map<AnimType, int> animType_;				

	void(Player::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^
};

