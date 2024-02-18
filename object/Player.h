#pragma once
#include "PlayerData.h"
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;
class Lever;

class Player
{
private:
	//�v�@6byte
	struct JumpInfo {
		float jumpVec = 0.0f;					//�W�����v�x�N�g��		//4byte
		bool isJump = false;					//�W�����v�����ǂ���	//2byte
	};

	//�v�@10byte
	struct Situation {
		bool isMoving = false;					//�ړ�����					//2byte			
		bool isClim = false;					//�o��邩					//2byte
		bool isInTransit = false;				//�^������					//2byte
		bool isCanBeCarried = false;			//�����^�Ԃ��Ƃ��o���邩	//2byte
		bool isGimmickCanBeOperated = false;	//�M�~�b�N�𑀍쒆��		//2byte
	};

	//�v�@62byte
	struct PlayerStatus {
		int animNo;								//�A�j���[�V�����ԍ�				//4byte
		float height;							//����								//4byte
		VECTOR pos;								//�|�W�V����						//12byte
		VECTOR rot;								//��]								//12byte
		VECTOR moveVec;							//�ړ��x�N�g��						//12byte
		JumpInfo jump;							//�W�����v							//6byte
		Situation situation;					//�V�`���G�[�V����					//10byte
		bool isAnimLoop;						//�A�j���[�V�����̃��[�v���K�v��	//2byte
	};

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="info">�z�u�f�[�^</param>
	Player(LoadObjectInfo info);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Player();

	/// <summary>
	/// �v���C���[�̍X�V���s��
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	/// <param name="models">�Փ˔�����s�����f����vector�^�̔z��</param>
	void Update(std::shared_ptr<ObjectManager> objManager);

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

	/// <summary>
	/// �x�N�g����ݒ肷��
	/// </summary>
	/// <param name="vector">�x�N�g��</param>
	void SetMoveVec(VECTOR vector) { status_.moveVec = vector; }

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
	void SetCrankPointer(std::shared_ptr<ManualCrank> crank);

	/// <summary>
	/// ���o�[�̃|�C���^��ݒ肷��
	/// </summary>
	void SetLeverPointer(std::shared_ptr<Lever> lever);

	/// <summary>
	/// ���f���|�C���^�[���擾����
	/// </summary>
	/// <param name="height"></param>
	/// <param name="materialType"></param>
	std::shared_ptr<Model> GetModelPointer() { return model_; }

	/// <summary>
	/// �e�ɓ���������m�b�N�o�b�N��ǉ�����
	/// </summary>
	void BulletHitMe(VECTOR moveVec);

	/// <summary>
	/// �n�ʂɕ`�悷��e�̍�����ݒ肷��
	/// </summary>
	/// <param name="height">����</param>
	/// <param name="materialType">������ł���I�u�W�F�N�g�����o�Ă��Ă��邩</param>
	void SetRoundShadowHeightAndMaterial(float height, Material materialType);

	/// <summary>
	/// �e��`�悷�鍂�����擾����
	/// </summary>
	/// <returns>�e��`�悷�鍂��</returns>
	float GetRoundShadowHeight() { return roundShadowHeight_; }

private:
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void NormalUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �A�j���[�V������idle�ɕύX����
	/// </summary>
	void ChangeAnimIdle();

	/// <summary>
	/// �ړ��̍X�V
	/// </summary>
	void MovingUpdate();

	/// <summary>
	/// �ړ�
	/// </summary>
	float Move();

	/// <summary>
	/// ��]����
	/// </summary>
	void RotationUpdate();

	void tempRotationUpdate();

	/// <summary>
	/// ����W�����v�ł͂Ȃ��Ƃ��̃W�����v
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	void JumpUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �v���C���[�̎��̂ɗ^����������֐�
	/// </summary>
	/// <param name="input">�O�����u�̓��͏����Q�Ƃ���</param>
	void DeathUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// ���̂̌㏈��
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void CorpsePostProsessing(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �v���C���[�̎��̂�vector�z��ŊǗ�����֐�
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void CorpseGenerater(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �ו����^��
	/// </summary>
	void CarryObject();

	/// <summary>
	/// �ו������낷
	/// </summary>
	void DropOffObject();

	/// <summary>
	/// �N�����N���񂷃|�W�V�����܂ōs��
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void GoCrankRotationPosition(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �N�����N�̍X�V
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void CrankUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �N�����N����]������A�b�v�f�[�g
	/// </summary>
	/// <param name="rotZ">Z�̉�]��</param>
	void CrankRotationUpdate(float rotZ);

	/// <summary>
	/// �������Ƃ̏Փ˃A�b�v�f�[�g
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param> 
	void BulletHitMeUpdate(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// ���o�[��|���|�W�V�����֍s��
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void GoLeverPullPosition(std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// ���o�[�̍X�V
	/// </summary>
	/// <param name="objManager">objManager�̃|�C���^</param>
	void LeverUpdate(std::shared_ptr<ObjectManager> objManager);

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
	/// ���ӂ݂̉�
	/// </summary>
	void FootStepSound();

	/// <summary>
	/// �A�j���[�V�����̕ύX���s��
	/// </summary>
	/// <param name="type">�A�j���[�V�����̃^�C�v</param>
	void ChangeAnimNo(PlayerAnimType type,bool isAnimLoop,int changeTime);

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

	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float angle_ = 0.0f;
	float roundShadowHeight_ = 0.0f;

	bool debugCreativeMode = false;

	VECTOR tentativeRot_ = {};

	VECTOR checkPoint_ = {0.0f,0.0f, 0.0f};						//���ԃ|�C���g
	VECTOR scale_ = {0.0f,0.0f, 0.0f};							//�g�k��

	PlayerInfo playerInfo_ = {};
	PlayerStatus status_ = {};

	Material materialSteppedOn_ = Material::Stone;

	std::shared_ptr<Model> model_;								//���f���N���X�̃|�C���^
	std::shared_ptr<ManualCrank> crank_;						//�N�����N�N���X�̃|�C���^
	std::shared_ptr<Lever> lever_;								//�N�����N�N���X�̃|�C���^
	std::shared_ptr<ObjectBase> deadPersonModelPointer_;		//�����^�Ԏ��̂̃��f���|�C���^

	void(Player::* updateFunc_)(std::shared_ptr<ObjectManager> objManager);		//�����o�֐��|�C���^
	void(Player::* carryUpdateFunc_)();		//�����o�֐��|�C���^
};

