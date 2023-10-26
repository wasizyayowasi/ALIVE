#include "Aster.h"
#include "../util/ObjectManager.h"
#include <algorithm>

namespace {
	//�񎟌��z��̍ő�X
	constexpr int max_X = 39 ;
	//�񎟌��z��̍ő�Z
	constexpr int max_Y = 6;

	//�񎟌��z��̍ő�v�f��
	constexpr int max_Index = max_X * max_Y;
}

Aster::Aster()
{
	for (int i = 0; i < 234;i++) {
		masu_[i].centerPos = VGet((i % max_X) * 100 - 850, 0, (i / max_X) * 100 - 250);
		masu_[i].masuMode = MasuMode::normalMode;
		masu_[i].x = i % max_X;
		masu_[i].z = i / max_X;
	}

}

Aster::~Aster()
{
}

void Aster::Update()
{
	
}

void Aster::Draw()
{
	for (float x = -900.0f; x < 3000.0f; x += 100.0f) {
		DrawLine3D(VGet(x, 20, 300), VGet(x, 20, -300), 0xff0000);
	}
	for (float z = -300.0f; z < 300.0f; z += 100.0f) {
		DrawLine3D(VGet(-900, 20, z), VGet(3000, 20, z), 0xff0000);
	}

	for (int i = 0; i < max_Index; i++) {
		DrawSphere3D(masu_[i].centerPos, 16, 32, 0x00ff00, 0x00ff00, true);
		VECTOR pos = ConvWorldPosToScreenPos(masu_[i].centerPos);
		DrawFormatString(pos.x, pos.y, 0x448844, "%d", i);
	}

	DrawFormatString(0, 16, 0x448844, "�G������C���f�b�N�X%d", enemyIndex_);
	DrawFormatString(0, 32, 0x448844, "�v���C���[������C���f�b�N�X%d", playerIndex_);
	DrawFormatString(0, 48, 0x448844, "�ړI�̃C���f�b�N�X%d", destination_.index);


	int y = 112;
	for (auto& result : score_) {
		DrawFormatString(0, y, 0x448844, "%d , score:%d , estination:%d , moveCost:%d",result.first, result.second.score, result.second.estimationCost, result.second.moveCost);
		y += 16;
	}

}

// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
VECTOR Aster::LocationInformation(VECTOR playerPos, VECTOR enemyPos)
{
	float min = 1000;

	for (int i = 0; i < max_Index; i++) {
		VECTOR distance = VSub(masu_[i].centerPos, enemyPos);
		float size = VSize(distance);
		if (min > size) {
			min = size;
			enemyIndex_ = i;
		}
	}

	min = 1000;
	for (int i = 0; i < max_Index; i++) {
		VECTOR distance = VSub(masu_[i].centerPos, playerPos);
		float size = VSize(distance);
		if (min > size) {
			min = size;
			playerIndex_ = i;
		}
	}

	return SearchAroundSquares();

}

/// ���͂̏������݂��邩�T��
VECTOR Aster::SearchAroundSquares()
{
	int leftEnd = enemyIndex_ % max_X;
	int rightEnd = enemyIndex_ % max_X;
	int bottom = enemyIndex_ / max_X;
	int top = enemyIndex_ / max_X;

	bool isCheckLeft = false;
	bool isCheckBottom = false;
	bool isCheckRight = false;
	bool isCheckTop = false;

	if (leftEnd == 0) {
		isCheckLeft = true;
	}
	if (bottom == 0) {
		isCheckBottom = true;
	}
	if (rightEnd == max_X - 1) {
		isCheckRight = true;
	}
	if (top == max_Y) {
		isCheckTop = true;
	}

	return SearchSurrroundingSquares(isCheckLeft, isCheckRight, isCheckTop, isCheckTop);

}

//����̏��̃X�R�A���擾����
void Aster::ScoreCaluculation(Direction direction, int index)
{
	//����R�X�g
	int estimationCostX = 0;
	int estimationCostZ = 0;

	//X��Z�ǂ��Ɉړ�������
	estimationCostX = masu_[playerIndex_].x - masu_[index].x;
	estimationCostZ = masu_[playerIndex_].z - masu_[index].z;

	//����R�X�g��direction�̔z��ɓ����
	if (estimationCostX < 0 || estimationCostZ < 0) {
		if (estimationCostX < 0) {
			estimationCostX = -(std::min)(estimationCostX, 0);
		}
		if (estimationCostZ < 0) {
			estimationCostZ = -(std::min)(estimationCostZ, 0);
		}
		
		score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}
	else {
		score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}

	//�ړ��ʂ��J�E���g����
	score_[direction].moveCost = moveCount_;
	//�ړ��R�X�g�Ɛ���R�X�g�̌��ʂ𑫂��ăX�R�A���o��
	score_[direction].score = score_[direction].moveCost + score_[direction].estimationCost;
	//�C���f�b�N�X������
	score_[direction].index = index;

}

//���͂̏��̃X�R�A���擾����
VECTOR Aster::SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom)
{
	//���͂̏��̃C���f�b�N�X
	int left			= enemyIndex_ - 1;
	int topLeft			= enemyIndex_ + max_X - 1;
	int top				= enemyIndex_ + max_X;
	int topRight		= enemyIndex_ + max_X + 1;
	int right			= enemyIndex_ + 1;
	int bottomRight		= enemyIndex_ - max_X + 1;
	int bottom			= enemyIndex_ - max_X;
	int bottomLeft		= enemyIndex_ - max_X - 1;

	if (!skipCheckLeft) {
		//���̏��̃X�R�A���擾����
		if (masu_[left].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::left, left);
		}
		//����̏��̃X�R�A���擾����
		if (masu_[topLeft].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::topLeft, topLeft);
		}
		//�����̏��̃X�R�A���擾����
		if (masu_[bottomLeft].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottomLeft, bottomLeft);
		}
	}

	if (!skipCheckRight) {
		//�E�̏��̃X�R�A���擾����
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);

		}
		//�E��̏��̃X�R�A���擾����
		if (masu_[topRight].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::topRight, topRight);
		}
		//�E���̏��̃X�R�A���擾����
		if (masu_[bottomRight].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottomRight, bottomRight);
		}
	}

	if (!skipCheckTop) {
		//��̏��̃X�R�A���擾����
		if (masu_[top].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::top, top);
		}
	}

	if (!skipCheckBottom) {
		//���̏��̃X�R�A���擾����
		if (masu_[bottom].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::bottom, bottom);
		}
	}

	//������
	destination_.score = 100;
	destination_.index = 0;
	//��ԃX�R�A���Ⴂ���ʂ��擾����
	for (auto& result : score_) {
		if (destination_.score > result.second.score) {
			destination_.score = result.second.score;
			destination_.index = result.second.index;
		}
	}

	//score_.clear();

	return masu_[destination_.index].centerPos;

}