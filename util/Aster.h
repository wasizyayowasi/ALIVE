#pragma once
#include "DxLib.h"
#include <unordered_map>
#include <list>

enum class MasuMode {
	normalMode,			//�ʏ탂�[�h
	passingMode,		//�ʉߍς݃��[�h
	blockadeMode,		//�������[�h
};

enum class Direction {
	left,				//��
	topLeft,			//����
	top,				//��
	topRight,			//�E��
	right,				//�E
	bottomRight,		//�E��
	bottom,				//��
	bottomLeft,			//����
};

struct MasuState {
	VECTOR centerPos;	//���S���W
	MasuMode masuMode;	//���̃��[�h
	int x;				//2�����z���X
	int z;				//2�����z���Y
};

struct Score {
	int moveCost;		//�ړ��R�X�g
	int estimationCost;	//����R�X�g
	int score;			//�v���C���[��ǐՂ��邤���œ��_��t����
	int index;			//�C���f�b�N�X
};

struct DesinationState {
	int index;
	int score;
};

class Aster
{
public:
	Aster();
	virtual ~Aster();

	//�X�V
	void Update();
	//�`��
	void Draw();

	/// <summary>
	/// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="pos">�|�W�V����</param>
	VECTOR LocationInformation(VECTOR playerPos,VECTOR enemyPos);

	/// <summary>
	/// ���͂̏�����������
	/// </summary>
	VECTOR SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom);

	/// <summary>
	/// ���͂̏������݂��邩�T��
	/// </summary>
	VECTOR SearchAroundSquares();

	/// <summary>
	/// ���̃X�R�A���擾����
	/// </summary>
	/// <param name="direction">����</param>
	/// <param name="index">���̔ԍ�</param>
	void ScoreCaluculation(Direction direction,int index);

	/// <summary>
	/// �v���C���[����G�܂ł̒��������̏��ɃI�u�W�F�N�g�����邩
	/// </summary>
	/// <param name="playerPos">�v���C���[�̍��W</param>
	/// <param name="enemyPos">�G�̍��W</param>
	/// <returns>���������ɃI�u�W�F�N�g�����邩</returns>
	bool StraightLineDistanceSearch(VECTOR playerPos, VECTOR enemyPos);

	/// <summary>
	/// ���W���珡�̃C���f�b�N�X���擾����
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	int SearchCurrentIndex(VECTOR pos);

private:

	int enemyIndex_ = 0;
	int playerIndex_ = 0;
	int moveCount_ = 1;

	DesinationState destination_ = {};

	std::unordered_map<int, MasuState> masu_;
	std::unordered_map<Direction, Score> score_;
	std::unordered_map<Direction, Score> tempscore_;

	std::list<int> preteriteIndex_;

};

