#pragma once
#include "DxLib.h"
//#include "CharacterBase.h"
#include <vector>
#include <unordered_map>
#include <memory>

class InputState;
class Model;
class CheckCollisionModel;

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

class Player
{
	friend CheckCollisionModel;
public:

	Player();
	virtual ~Player();

	void update(const InputState& input, std::vector<std::shared_ptr<Model>> models);
	void draw();

	VECTOR getPos() { return pos_; }
	VECTOR getRot();
	void setPos(VECTOR pos);
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

	void idleUpdate(const InputState& input);

private:
	//�ړ�����
	void movingUpdate(const InputState& input);
	//�W�����v����
	void jumpUpdate(const InputState& input);
	//����W�����v����
	void runningJumpUpdate(const InputState& input);
	//���S����
	void death(const InputState& input);
	//�ҋ@����
	void changeAnimIdle();
	//��]����
	void rotationUpdate();
	//���l����
	void deadPersonGenerater();
	//����
	void sitUpdate(const InputState& input);
	//�I�u�W�F�N�g��o��
	void climUpdate(const InputState& input);
	//�����オ��
	void standUpdate(const InputState& input);

private:

	int temp = 0;
	float tempGravity = 0.0f;

	int animNo_ = 0;						//���݂̃A�j���[�V�����ԍ�
	

	float movingSpeed_ = 0.0f;				//�ړ����x
	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float tempAngle_ = 0.0f;

	bool isMoving = false;					//�ړ�����
	bool isDead_ = false;					//����ł��邩
	bool isSitting_ = false;				//�����Ă��邩
	bool isAnimLoop_ = true;				//�A�j���[�V���������[�v�����邩
	bool isClim_ = false;

	JumpInfo jump_;							//�W�����v�֘A�̍\����


	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };					//���ԃ|�C���g

	VECTOR pos_ = { 0.0f,0.0f,0.0f };							//�v���C���[�̃|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };							//�v���C���[�̉�]
	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };						//�v���C���[�̈ړ��x�N�g��
	VECTOR temp_ = { 0.0f,0.0f,0.0f };						//�v���C���[�̈ړ��x�N�g��

	VECTOR deathPos = { 0.0f,0.0f,0.0f };						//���̂̃|�W�V����

	std::shared_ptr<Model> PModel_;								//���f���N���X�̃|�C���^
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//�Փ˔�����s���N���X�̃|�C���^
	std::vector<std::shared_ptr<Model>> deadPlayer_;			//���̂�ۑ����邽��

	std::unordered_map<AnimType, int> animType_;				

	PlayerInfo playerInfo;

	void(Player::* updateFunc_)(const InputState& input);		//�����o�֐��|�C���^
};

