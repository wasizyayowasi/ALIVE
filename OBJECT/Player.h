#pragma once
#include "DxLib.h"
#include "CharacterBase.h"
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

enum class AnimType {
	idle,			//�ҋ@���
	run,			//����
	stairs,			//�K�i�����
	radder,			//���
	jump,			//�W�����v
	runningJump,	//����W�����v
	death,			//����W�����v
	walk,			//����
	idleToSitup,	//idle��Ԃ������
	situpToIdle,	//�����Ă����Ԃ���idle
	sit,			//�����Ă���
	clim,			//�o��
	stand,			//�����オ��
	carry,			//�^��
	carryWalking,	//�^�тȂ������
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int anim_[static_cast<int>(AnimType::max)];
};

class Player:public CharacterBase
{
	friend CheckCollisionModel;
	friend GimmickBase;
public:

	Player(const char* const filename);
	Player(int handle);
	virtual ~Player();

	void init();

	void update(const InputState& input);
	void draw();

	VECTOR getPos() { return pos_; }
	VECTOR getRot();
	void setPos(VECTOR pos);
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

	VECTOR getMoveVec() { return moveVec_; }

	void setSaveData(VECTOR pos, int num, bool isContinue);
	int getDeathNum() {return deathCount_;	}
	std::list<std::shared_ptr<Model>> getDeadPerson() { return deadPerson_; }
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

	//�����オ��
	void standUpdate(const InputState& input);

	//�����^��
	void carryObject();

private:

	/// <summary>
	/// ���S�p�^�[���ʁA�Փˏ��̐ݒ�
	/// </summary>
	void setCollitionInfoByDeathPattern();

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

	JumpInfo jump_;												//�W�����v�֘A�̍\����


	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//���ԃ|�C���g

	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };						//�v���C���[�̈ړ��x�N�g��
	VECTOR temp_ = { 0.0f,0.0f,0.0f };							//�v���C���[�̈ړ��x�N�g��

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//���̂̃|�W�V����

	std::shared_ptr<Model> PModel_;								//���f���N���X�̃|�C���^
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^

	std::list<std::shared_ptr<Model>> deadPerson_;				//���̂�ۑ����邽��

	std::unordered_map<AnimType, int> animType_;				

	PlayerInfo playerInfo;

	void(Player::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^
};

