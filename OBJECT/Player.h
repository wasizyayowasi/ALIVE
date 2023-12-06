#pragma once
#include "CharacterBase.h"
#include "util/PlayerData.h"
#include "util/ObjectData.h"
#include <DxLib.h>
#include <list>
#include <unordered_map>
#include <memory>

class InputState;
class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;

struct JumpInfo {
	float jumpVec = 0.0f;					//�W�����v�x�N�g��
	bool isJump = false;					//�W�����v�����ǂ���
};

struct Situation {
	bool isMoving = false;					//�ړ�����
	bool isSitting = false;					//�����Ă��邩
	bool isClim = false;					//�o��邩
	bool isInTransit = false;				//�^������
	bool isCanBeCarried = false;			//�����^�Ԃ��Ƃ��o���邩
	bool isGimmickCanBeOperated = false;	//�M�~�b�N�𑀍쒆��
};

struct PlayerStatus {
	VECTOR pos;
	VECTOR rot;
	VECTOR moveVec;
	float height;
	int animNo;
	bool isAnimLoop;
	JumpInfo jump;
	Situation situation;
};

class Player
{
public:

	Player(const char* const filename);
	virtual ~Player();

	void Init(LoadObjectInfo info);

	/// <summary>
	/// �v���C���[�̍X�V���s��
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	/// <param name="models">�Փ˔�����s�����f����vector�^�̔z��</param>
	void Update(const InputState& input,std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �v���C���[�֘A�̕`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���񂾉񐔂��擾����
	/// </summary>
	int GetDeathCount() { return deathCount_; }

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

	void SetMoveVec(VECTOR vector) { status_.moveVec = vector; }

	/// <summary>
	/// �o��邩�̏����󂯎��
	/// </summary>
	/// <returns>�o��邩�̃t���O</returns>
	void SetClim(bool isClim) { status_.situation.isClim = isClim; }

	/// <summary>
	/// �����^�Ԏ����o����t���O�Ǝ����^�ԃ��f���̃|�C���^���󂯎��
	/// </summary>
	void SetCarryInfo(bool isCarry, std::shared_ptr<ObjectBase> model);

	/// <summary>
	/// �v���C���[�̃X�e�[�^�X���擾����
	/// </summary>
	PlayerStatus GetStatus() { return status_; }

	/// <summary>
	/// �����^�Ԏ��̂̃|�C���^�[���擾����
	/// </summary>
	std::shared_ptr<ObjectBase> GetDeadPersonModelPointer() {return deadPersonModelPointer_;}
	
	/// <summary>
	/// ManualCrank�̃|�C���^���擾����
	/// </summary>
	/// <param name="crank"></param>
	void SetGimmickModelPointer(std::shared_ptr<ManualCrank> crank);

	std::shared_ptr<Model> GetModelPointer() { return model_; }

	void BulletHitMe(VECTOR moveVec);

	void SetRoundShadowHeight(float height) { roundShadowHeight_ = height; }

private:
	//�ʏ�X�V
	void NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//�ҋ@����
	void ChangeAnimIdle();

	//�ړ�����
	void MovingUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//�ړ�
	float Move(const InputState& input);

	//��]����
	void RotationUpdate();

	//�W�����v����
	void JumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//���S����
	void DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//���̂̌㏈��
	void DeathPersonPostProsessing(std::shared_ptr<ObjectManager> objManager);

	//���l����
	void DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager);

	//����
	void SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//�����Ă���r��
	void IdleToSitup(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//�����^��
	void CarryObjectUpdate();

	//�ו������낷
	void DropOffObjectUpdate();

	//�N�����N����
	void GoCrankRotationPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	void CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// �N�����N����]������A�b�v�f�[�g
	/// </summary>
	void CrankRotatinUpdate(float rotZ);

	/// <summary>
	/// �������Ƃ̏Փ˃A�b�v�f�[�g
	/// </summary>
	void BulletHitMeUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

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
	/// <returns>�|�W�V����</returns>
	VECTOR CenterFramPosition(const char* const LeftFramename, const char* const RightFramename);

	/// <summary>
	/// �w�肵���t���[���̒��S���W���Z�o����
	/// </summary>
	/// <returns>�|�W�V����</returns>
	VECTOR FramPosition(const char* const framename);

	/// <summary>
	/// �A�j���[�V�����̕ύX���s��
	/// </summary>
	/// <param name="type">�A�j���[�V�����̃^�C�v</param>
	void ChangeAnimNo(PlayerAnimType type,bool isAnimLoop,int changeTime);

	/// <summary>
	/// �x���@����ʓx�@�ɕϊ����ĕԂ�
	/// </summary>
	/// <param name="rot">�p�x</param>
	/// /// <returns>�|�W�V����</returns>
	VECTOR DegreesToRadians(VECTOR rot);

	/// <summary>
	/// �����e�̕`��
	/// </summary>
	void DrawPolygon3D();

	/// <summary>
	/// �����e�̒��_�̃|�W�V�������擾����
	/// </summary>
	/// <param name="angle">�p�x</param>
	/// <returns>�|�W�V����</returns>
	VECTOR VertexPosition(float angle);

private:

	int deathCount_ = 0;					//���񂾉񐔂��L�^����

	float totalAnimFrame_ = 0;
	float runJumpGravity = 0.0f;

	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float angle_ = 0.0f;
	float roundShadowHeight_ = 0.0f;

	bool debugCreativeMode = false;

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//���ԃ|�C���g
	VECTOR scale_ = {0.0f,0.0f, 0.0f};							//�g�k��

	std::shared_ptr<Model> model_;								//���f���N���X�̃|�C���^
	std::shared_ptr<ManualCrank> crank_;						//�N�����N�N���X�̃|�C���^
	std::shared_ptr<ObjectBase> deadPersonModelPointer_;		//�����^�Ԏ��̂̃��f���|�C���^

	void(Player::* updateFunc_)(const InputState& input, std::shared_ptr<ObjectManager> objManager);		//�����o�֐��|�C���^
	void(Player::* carryUpdateFunc_)();		//�����o�֐��|�C���^
};

