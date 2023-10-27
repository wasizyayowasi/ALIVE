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
		masu_[i].centerPos = VGet((i % max_X) * 100.0f - 850.0f, 0.0f, (i / max_X) * 100.0f - 250.0f);
		masu_[i].masuMode = MasuMode::normalMode;
		masu_[i].x = i % max_X;
		masu_[i].z = i / max_X;
	}

	masu_[9].masuMode = MasuMode::blockadeMode;
	masu_[48].masuMode = MasuMode::blockadeMode;
	masu_[87].masuMode = MasuMode::blockadeMode;
	masu_[126].masuMode = MasuMode::blockadeMode;
//	masu_[165].masuMode = MasuMode::blockadeMode;
	masu_[204].masuMode = MasuMode::blockadeMode;

//	masu_[46].masuMode = MasuMode::blockadeMode;
//	masu_[47].masuMode = MasuMode::blockadeMode;
//	masu_[48].masuMode = MasuMode::blockadeMode;
//	masu_[85].masuMode = MasuMode::blockadeMode;
//	masu_[86].masuMode = MasuMode::blockadeMode;
//	masu_[87].masuMode = MasuMode::blockadeMode;
//	masu_[124].masuMode = MasuMode::blockadeMode;
//	masu_[125].masuMode = MasuMode::blockadeMode;
//	masu_[126].masuMode = MasuMode::blockadeMode;

}

Aster::~Aster()
{
}

void Aster::Update()
{
	
}

void Aster::Draw()
{
	//�f�o�b�O�p�`��
	{
		for (float x = -900.0f; x < 3000.0f; x += 100.0f) {
			DrawLine3D(VGet(x, 20, 300), VGet(x, 20, -300), 0xff0000);
		}
		for (float z = -300.0f; z < 300.0f; z += 100.0f) {
			DrawLine3D(VGet(-900, 20, z), VGet(3000, 20, z), 0xff0000);
		}

		for (int i = 0; i < max_Index; i++) {
			if (masu_[i].masuMode == MasuMode::normalMode) {
				DrawSphere3D(masu_[i].centerPos, 16, 32, 0x00ff00, 0x00ff00, true);
			}
			else if(masu_[i].masuMode == MasuMode::blockadeMode) {
				DrawSphere3D(masu_[i].centerPos, 16, 32, 0xff0000, 0xff0000, true);
			}
			else {
				DrawSphere3D(masu_[i].centerPos, 16, 32, 0xffff00, 0xffff00, true);
			}
			
			VECTOR pos = ConvWorldPosToScreenPos(masu_[i].centerPos);
			DrawFormatString(pos.x, pos.y, 0x448844, "%d", i);
		}

		DrawFormatString(0, 16, 0x448844, "�G������C���f�b�N�X%d", enemyIndex_);
		DrawFormatString(0, 32, 0x448844, "�v���C���[������C���f�b�N�X%d", playerIndex_);
		DrawFormatString(0, 48, 0x448844, "�ړI�̃C���f�b�N�X%d", destination_.index);

		//int y = 128;
		for (auto& result : tempscore_) {
			VECTOR pos = ConvWorldPosToScreenPos(masu_[result.second.index].centerPos);
			DrawFormatString(pos.x, pos.y + 10, 0xff0000, "S:%d", result.second.score);
			//y += 16;
		}
	}
}

// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
VECTOR Aster::LocationInformation(VECTOR playerPos, VECTOR enemyPos)
{
	

	int oldEnemyIndex = enemyIndex_;

	//�G�̃C���f�b�N�X���擾
	enemyIndex_ = SearchCurrentIndex(enemyPos);

	//��O�ɂ����C���f�b�N�X���W�̏��̃��[�h��ʉߍς݂ɂ���
	if (oldEnemyIndex != enemyIndex_) {
		if (masu_[oldEnemyIndex].masuMode != MasuMode::blockadeMode) {
			//��O�̏���ʉߍς݂ɂ���
			masu_[oldEnemyIndex].masuMode = MasuMode::passingMode;
			//�ʉ߂������̃C���f�b�N�X��ۑ�����
			preteriteIndex_.push_back(oldEnemyIndex);
			//�ۑ��������̃C���f�b�N�X��2�ȏ�ɂȂ�����
			//��ԌÂ�����Mode��normal�ɂ��A��ԌÂ��f�[�^���폜����
			if (preteriteIndex_.size() > 10) {
				masu_[preteriteIndex_.front()].masuMode = MasuMode::normalMode;
				preteriteIndex_.remove(preteriteIndex_.front());
			}
		}
	}

	//�v���C���[�̃C���f�b�N�X���擾
	playerIndex_ = SearchCurrentIndex(playerPos);

	//�v���C���[�ƓG�̃C���f�b�N�X�������������ꍇ
	//���̃}�X�̒��S���W��Ԃ�
	if (playerIndex_ == enemyIndex_) {
		for (auto& mode : masu_) {
			if (mode.second.masuMode != MasuMode::blockadeMode) {
				mode.second.masuMode = MasuMode::normalMode;
			}
		}
		return masu_[playerIndex_].centerPos;
	}

	return SearchAroundSquares();

}

/// ���͂̏������݂��邩�T��
VECTOR Aster::SearchAroundSquares()
{
	//���[�A�㉺�̒[�̃}�X���擾
	int leftEnd = enemyIndex_ % max_X;
	int rightEnd = enemyIndex_ % max_X;
	int bottom = enemyIndex_ / max_X;
	int top = enemyIndex_ / max_X;

	//�}�X���Ȃ��Ƃ���͌��Ȃ��悤�ɂ���
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

bool Aster::StraightLineDistanceSearch(VECTOR playerPos, VECTOR enemyPos)
{
	VECTOR distance = VSub(playerPos, enemyPos);

	float size = VSize(distance);
	int num = size / 100;
	bool isBlockade = false;

	//�G�̃C���f�b�N�X���擾
	enemyIndex_ = SearchCurrentIndex(enemyPos);
	//�v���C���[�̃C���f�b�N�X���擾
	playerIndex_ = SearchCurrentIndex(playerPos);

	for (int i = 0; i < num; i++) {
		distance = VSub(playerPos, enemyPos);
		VECTOR norm = VNorm(distance);
		VECTOR scale = VScale(norm, i * 100.0f);
		VECTOR pos = VAdd(enemyPos, scale);

		int x = pos.x / 100;
		int z = pos.z / 100;

		int temp = enemyIndex_ + (x * max_X + z);

		//masu_[temp].masuMode = MasuMode::blockadeMode;
		if (masu_[x + z * max_X].masuMode == MasuMode::blockadeMode) {
			isBlockade = true;
		}

	}

	return isBlockade;
}

int Aster::SearchCurrentIndex(VECTOR pos)
{

	int index = 0;
	float min = 1000.0f;
	float size = 0.0f;
	VECTOR distance = {};

	//���W�����ԋ߂��}�X�̒��S������index�ԍ����擾����
	for (int i = 0; i < max_Index; i++) {
		//���S����G�̃|�W�V�����̋������Ƃ�
		distance = VSub(masu_[i].centerPos, pos);
		size = VSize(distance);
		//�ߋ��ŒZ�ɋ߂����ʂƔ�ׂ�
		if (min > size) {
			//�ߋ��ŒZ���X�V
			min = size;
			//�C���f�b�N�X���擾
			index = i;
		}
	}

	//�C���f�b�N�X���擾
	return index;
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

	if (!skipCheckLeft) {
		//���̏��̃X�R�A���擾����
		if (masu_[left].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::left, left);
		}
		////����̏��̃X�R�A���擾����
		//if (masu_[topLeft].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::topLeft, topLeft);
		//}
		////�����̏��̃X�R�A���擾����
		//if (masu_[bottomLeft].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::bottomLeft, bottomLeft);
		//}
	}

	if (!skipCheckRight) {
		//�E�̏��̃X�R�A���擾����
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);
		}
		////�E��̏��̃X�R�A���擾����
		//if (masu_[topRight].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::topRight, topRight);
		//}
		////�E���̏��̃X�R�A���擾����
		//if (masu_[bottomRight].masuMode == MasuMode::normalMode) {
		//	ScoreCaluculation(Direction::bottomRight, bottomRight);
		//}
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

	//�f�o�b�O�p
	tempscore_ = score_;

	score_.clear();

	return masu_[destination_.index].centerPos;

}