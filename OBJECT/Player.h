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
	bool isAnimLoop;
};

class Player
{
public:

	Player(const char* const filename);
	Player(int handle);
	virtual ~Player();

	void init();

	/// <summary>
	/// �v���C���[�̍X�V���s��
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	/// <param name="models">�Փ˔�����s�����f����vector�^�̔z��</param>
	void update(const InputState& input);

	/// <summary>
	/// �v���C���[�֘A�̕`��
	/// </summary>
	void draw();

	VECTOR getRot();

	/// <summary>
	/// �O������̃|�W�V�������󂯎��
	/// </summary>
	/// <param name="pos">�|�W�V�������</param>
	void setPos(VECTOR pos);

	/// <summary>
	/// �O������̃W�����v�����󂯎��
	/// </summary>
	/// <param name="isJump">�W�����v�t���O</param>
	/// <param name="jumpVec">�W�����v�x�N�g��</param>
	void setJumpInfo(bool isJump, float jumpVec);

	void setClim(bool isClim) { isClim_ = isClim; }

	void setSaveData(VECTOR pos, int num, bool isContinue);
	int getDeathNum() {return deathCount_;	}

	PlayerStatus getStatus() { return status_; }

private:
	//�ʏ�X�V
	void idleUpdate(const InputState& input);

	//�ҋ@����
	void changeAnimIdle();

	//�ړ�����
	void movingUpdate(const InputState& input);

	//��]����
	void rotationUpdate();

	//�I�u�W�F�N�g��o��
	void climUpdate(const InputState& input);

	//�W�����v����
	void jumpUpdate(const InputState& input);

	//����W�����v����
	void runningJumpUpdate(const InputState& input);

	//���S����
	void deathUpdate(const InputState& input);

	//���̂̌㏈��
	void deathPersonPostProsessing();

	//���l����
	void deadPersonGenerater();

	//����
	void sitUpdate(const InputState& input);

	//�����Ă���r��
	void idleToSitup(const InputState& input);

	//�����オ��
	void standUpdate(const InputState& input);

	//�����^��
	void carryObject();

private:
	/// <summary>
	/// �v���C���[�̈ړ����x��ݒ肷��
	/// </summary>
	/// <returns>float�^�̈ړ����x</returns>
	float playerSpeed(bool pressedShift);

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
	bool isContinue_ = false;

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//���ԃ|�C���g

	VECTOR temp_ = { 0.0f,0.0f,0.0f };							//�v���C���[�̈ړ��x�N�g��

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//���̂̃|�W�V����

	std::shared_ptr<Model> PModel_;								//���f���N���X�̃|�C���^
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^

	std::unordered_map<AnimType, int> animType_;				


	void(Player::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^
};

