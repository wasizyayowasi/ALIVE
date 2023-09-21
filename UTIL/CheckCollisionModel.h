#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>
#include <deque>

class Player;
class Model;

constexpr int max_hit_coll = 2048;

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
	void checkCollisionPersonalArea(Player& player,VECTOR moveVec, std::vector<std::shared_ptr<Model>> models);

	/// <summary>
	/// �Փ˂������f���̃|���S�����ǂ��𔻒f����
	/// </summary>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void checkCollisionWall(VECTOR moveVec, float playerHeight);

	/// <summary>
	/// �Փ˂������f���̃|���S���������𔻒f����
	/// </summary>
	/// <param name="player">�v���C���[���Q��</param>
	/// <param name="moveVec">�v���C���[�̈ړ���</param>
	/// <param name="jumpFlag">�v���C���[�̃W�����v�t���O</param>
	/// <param name="playerHeight">�v���C���[�̍���</param>
	void checkCollisionFloor(Player& player, VECTOR moveVec,bool jumpFlag,float playerHeight);

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
	void checkCollision(Player& player, VECTOR moveVec, std::vector<std::shared_ptr<Model>> model,float playerHeight,bool isJump,float jumpVec);

private:

	int objectHeightY = 0;

	bool temp = false;

	VECTOR oldPos;				//���݂̃v���C���[�̍��W
	VECTOR nowPos;				//�v���C���[�̈ړ��ʂƌ��݂̍��W�𑫂��Č���
	int i, j, k;
	int hitWallNum;				//�ǂƏՓ˂����|���S���̐�
	int hitFloorNum;			//���ƏՓ˂����|���S���̐�
	bool moveFlag = false;		//���݈ړ����Ă��邩�̃t���O
	bool hitFlag = false;		//�Փ˂������̃t���O

	//���f���Ƃ̓����蔻��p���\�b�h
	std::vector<MV1_COLL_RESULT_POLY_DIM> hitDim_;
	MV1_COLL_RESULT_POLY* wallHitDim_[max_hit_coll];
	MV1_COLL_RESULT_POLY* floorHitDim_[max_hit_coll];
	MV1_COLL_RESULT_POLY* hitPoly;
	HITRESULT_LINE hitLineResult;
};

