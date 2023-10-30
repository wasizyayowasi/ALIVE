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

}

Aster::~Aster()
{
}

void Aster::Init()
{
	route_.clear();
	scoreTable_.clear();
	for (auto& masu : masu_) {
		if (masu.second.masuMode == MasuMode::doneMode) {
			masu.second.masuMode = MasuMode::normalMode;
		}
	}
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
		if (!route_.empty()) {
			DrawFormatString(0, 48, 0x448844, "�ړI�̃C���f�b�N�X%d", route_.front());
		}

		//int y = 128;
		for (auto& result : debugScoreTable) {
			VECTOR pos = ConvWorldPosToScreenPos(masu_[result.second.destinationIndex].centerPos);
			DrawFormatString(pos.x, pos.y + 10, 0xff0000, "S:%d", result.second.score);
			//y += 16;
		}
	}
}

// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
void Aster::LocationInformation(VECTOR playerPos, VECTOR enemyPos)
{
	int oldEnemyIndex = enemyIndex_;
	int oldPlayerIndex = playerIndex_;

	//�G�̃C���f�b�N�X���擾
	enemyIndex_ = SearchCurrentIndex(enemyPos);

	//�v���C���[�̃C���f�b�N�X���擾
	playerIndex_ = SearchCurrentIndex(playerPos);

	//�v���C���[�ƓG�̃C���f�b�N�X�������������ꍇ
	//���̃}�X�̒��S���W��Ԃ�
	if (playerIndex_ == enemyIndex_) {
		for (auto& mode : masu_) {
			if (mode.second.masuMode != MasuMode::blockadeMode) {
				mode.second.masuMode = MasuMode::normalMode;
				moveCount_ = 0;
			}
		}
	}

	//�v���C���[�̃C���f�b�N�X���W���O�̃t���[���Ɣ�ׂ��Ƃ�
	//�������A�ēx�o�H�T�����s��
	if (oldPlayerIndex != playerIndex_ && masu_[playerIndex_].masuMode != MasuMode::blockadeMode) {
		Init();
		RouteSearch();
	}

}

/// ���͂̏������݂��邩�T��
void Aster::SearchAroundSquares()
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

	SearchSurrroundingSquares(isCheckLeft, isCheckRight, isCheckTop, isCheckBottom);

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
		
		//�l�����̎�
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;

		//�������̎�
		//score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}
	else {
		//�l�����̎�
		scoreTable_[index].estimationCost = estimationCostX + estimationCostZ;

		//�������̎�
		//score_[direction].estimationCost = (std::max)(estimationCostX, estimationCostZ);
	}

	//�ړ��ʂ��J�E���g����
	if (masu_[index].masuMode == MasuMode::normalMode) {
		scoreTable_[index].moveCost = moveCount_;
		scoreTable_[index].score = scoreTable_[index].moveCost + scoreTable_[index].estimationCost;
		scoreTable_[index].destinationIndex = index;
		scoreTable_[index].currentIndex = enemyIndex_;
		scoreTable_[index].dir = direction;
	}

	preteriteIndex_[moveCount_].push_back(index);

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

void Aster::RouteSearch()
{

	int currentIndex = enemyIndex_;

	while (enemyIndex_ != playerIndex_)
	{
		if (masu_[enemyIndex_].masuMode != MasuMode::blockadeMode) {
			masu_[enemyIndex_].masuMode = MasuMode::doneMode;
		}

		SearchAroundSquares();

		//���ɃX�R�A���o���}�X�����ɃX�R�A���o���Ă������ꍇ
		//����ڂɃX�R�A�v�Z���s��ꂽ�����擾����
		for (auto& list : preteriteIndex_) {
			for (auto& index : list.second) {
				if (index == enemyIndex_) {
					moveCount_ = list.first;
				}
			}
		}
		moveCount_++;
	}

	int targetIndex = playerIndex_;

	//�ŒZ���[�g��scoreTable���璊�o����
	while (currentIndex != targetIndex)
	{
		targetIndex = scoreTable_[targetIndex].currentIndex;
		route_.push_front(targetIndex);
	}
	//�Ō�Ƀv���C���[������C���f�b�N�X���W��ǉ�����
	route_.push_back(playerIndex_);

	//���Z�b�g
	scoreTable_.clear();
	debugScoreTable.clear();

}

VECTOR Aster::GetDestinationCoordinates(VECTOR playerPos,VECTOR enemyPos)
{
	//�G�l�~�[�ƃv���C���[�̃C���f�b�N�X���W���擾
	int enemyIndex = SearchCurrentIndex(enemyPos);
	int playerIndex = SearchCurrentIndex(playerPos);

	if (masu_[playerIndex].masuMode == MasuMode::blockadeMode) {
		return masu_[enemyIndex].centerPos;
	}

	//�v���C���[�ƃG�l�~�[�������C���f�b�N�X���W�ɋ���ꍇ
	if (enemyIndex == playerIndex) {
		//�ŒZ���[�g���폜����
		route_.clear();

		//�ŒZ���[�g��T���ۂɃ}�X�̃��[�h��doneMode�ɂ�������
		//doneMode����normalMode�ɖ߂�
		for (auto& masu : masu_) {
			if (masu.second.masuMode == MasuMode::doneMode) {
				masu.second.masuMode = MasuMode::normalMode;
			}
		}

		//�v���C���[�̋���C���f�b�N�X���W�̒��S�̃|�W�V������Ԃ�
		return masu_[playerIndex].centerPos;
	}

	//�ڕW�C���f�b�N�X���W�̒��S�|�W�V����
	VECTOR targetPos = masu_[route_.front()].centerPos;

	//�G�̃C���f�b�N�X���W�ƌo�H�T���œ����C���f�b�N�X�������������ꍇ
	//�����C���f�b�N�X���W���폜����
	if (enemyIndex == route_.front()) {
		route_.pop_front();
	}

	//�ڕW�|�W�V������Ԃ�
	return targetPos;
}

bool Aster::temp(VECTOR pos)
{
	int pointIndex = SearchCurrentIndex(pos);

	if (masu_[pointIndex].masuMode == MasuMode::blockadeMode) {
		return true;
	}

	return false;
}

bool Aster::EndOfDirection()
{
	auto aiu = route_.empty();
	return route_.empty();
}

//���͂̏��̃X�R�A���擾����
void Aster::SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom)
{
	//���͂̏��̃C���f�b�N�X
	int left			= enemyIndex_ - 1;
	int right			= enemyIndex_ + 1;
	int top				= enemyIndex_ + max_X;
	int bottom			= enemyIndex_ - max_X;

	int topLeft			= enemyIndex_ + max_X - 1;
	int topRight		= enemyIndex_ + max_X + 1;
	int bottomRight		= enemyIndex_ - max_X + 1;
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
	}

	if (!skipCheckRight) {
		//�E�̏��̃X�R�A���擾����
		if (masu_[right].masuMode == MasuMode::normalMode) {
			ScoreCaluculation(Direction::right, right);
		}
	}

	//������
	int score = 1000;

	//�ŒZ���[�g�����߂�ۂɈ�ԒႢ�X�R�A�̃C���f�b�N�X�}�X�����̒T���}�X�ɂ���
	for (auto& result : scoreTable_) {
		if (masu_[result.second.destinationIndex].masuMode == MasuMode::normalMode) {
			if (score > result.second.score) {
				score = result.second.score;
				enemyIndex_ = result.second.destinationIndex;
				count_++;
			}
		}
	}

	//�f�o�b�O�p
	debugScoreTable = scoreTable_;

}