#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>
#include <list>
#include <deque>

class Player;
class Model;

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
	void CheckCollisionPersonalArea(Player& player,VECTOR moveVec);

	/// <summary>
	/// �Փ˂����ǂƏ��̃|���S�����𐔂���
	/// </summary>
	void CheckWallAndFloor();

	/// <summary>
	/// �Փ˂������f���̃|���S�����ǂ��𔻒f���A�ړ��x�N�g����␳����
	/// </summary>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void CheckCollisionWall(VECTOR moveVec, float playerHeight);

	/// <summary>
	/// �Փ˂������f���̃|���S���������𔻒f����
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="jumpFlag">�v���C���[�̃W�����v�t���O</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void CheckCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag,float playerHeight);

	/// <summary>
	/// checkCollisionPersonalArea�AcheckCollisionWall�AcheckCollisionFloor���Ăяo���B
	/// �Ō�A��L�̊֐��Ŏ擾�����l����������
	/// </summary>
	/// <param name="player">�v���C���[���Q�Ƃ���</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="model">�Փ˔�����s�����f��</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	/// <param name="isJump">�v���C���[�̃W�����v�t���O</param>
	/// <param name="jumpVec">�v���C���[�̃W�����v�x�N�g��</param>
	void CheckCollision(Player& player, VECTOR moveVec,float playerHeight,bool isJump,float jumpVec);

	/// <summary>
	/// �Փ˂����I�u�W�F�N�g�����z�����邩���f����
	/// </summary>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void CheckStepDifference(Player& player, float playerHeight);


	void CheckCollSpecificModel(Player& player);

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
	//MV1_COLL_RESULT_POLY* hitPoly = {};
	HITRESULT_LINE hitLineResult = {};
};

