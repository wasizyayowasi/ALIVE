#pragma once
#include <DxLib.h>
#include <memory>
#include <vector>
#include <list>
#include <deque>

class Player;
class Model;
class ObjectManager;

constexpr int max_hit_coll = 2048;

struct CollModelState {
	MV1_COLL_RESULT_POLY_DIM hitDim;
	std::shared_ptr<Model> model;
};

struct CollResultPoly {
	MV1_COLL_RESULT_POLY* hitDim;
	std::shared_ptr<Model> model;
};

class CheckCollisionModel
{
public:

	//�R���X�g���N�^
	CheckCollisionModel();
	//�f�X�g���N�^
	virtual ~CheckCollisionModel();

	/// <summary>
	/// ����������͈͂̃I�u�W�F�N�g�Ƃ̏Փ˔���𓾂�
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="models">�v���C���[�ƏՓ˔�����s�����f��</param>
	void CheckCollisionPersonalArea(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �Փ˂����ǂƏ��̃|���S�����𐔂���
	/// </summary>
	void CheckWallAndFloor();

	/// <summary>
	/// �Փ˂������f���̃|���S�����ǂ��𔻒f���A�ړ��x�N�g����␳����
	/// </summary>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void CheckCollisionWall(std::shared_ptr<Player> player);

	/// <summary>
	/// �Փ˂������f���̃|���S���������𔻒f����
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="jumpFlag">�v���C���[�̃W�����v�t���O</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void CheckCollisionFloor(std::shared_ptr<Player> player);

	/// <summary>
	/// checkCollisionPersonalArea�AcheckCollisionWall�AcheckCollisionFloor���Ăяo���B
	/// �Ō�A��L�̊֐��Ŏ擾�����l����������
	/// </summary>
	/// <param name="player">�v���C���[���Q�Ƃ���</param>
	void CheckCollision(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// �Փ˂����I�u�W�F�N�g�����z�����邩���f����
	/// </summary>
	void CheckStepDifference(std::shared_ptr<Player> player);


	void CheckCollSpecificModel(std::shared_ptr<Player> player, std::shared_ptr<ObjectManager> objManager);

private:

	float objectHeightY = 0;		//�Փ˂����I�u�W�F�N�g�̍�����ۊǂ���

	bool isGoUpStep_ = false;	//�i������邱�Ƃ��o����

	VECTOR oldPos;				//���݂̃v���C���[�̍��W
	VECTOR nowPos;				//�v���C���[�̈ړ��ʂƌ��݂̍��W�𑫂��Č���
	int hitWallNum = 0;			//�ǂƏՓ˂����|���S���̐�
	int hitFloorNum = 0;		//���ƏՓ˂����|���S���̐�
	bool moveFlag = false;		//���݈ړ����Ă��邩�̃t���O
	bool hitFlag = false;		//�Փ˂������̃t���O

	//���f���Ƃ̓����蔻��p���\�b�h
	std::list<CollModelState> hitDim_;
	CollResultPoly wallHitDim_[max_hit_coll] = {};
	CollResultPoly floorHitDim_[max_hit_coll] = {};
	HITRESULT_LINE hitLineResult = {};
};

