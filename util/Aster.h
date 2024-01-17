#pragma once
#include <DxLib.h>
#include <unordered_map>
#include <list>



class Aster
{
private:

	enum class MasuMode {
		normalMode,				//�ʏ탂�[�h
		doneMode,				//�ς݃��[�h
		blockadeMode,			//�������[�h
	};

	enum class Direction {
		left,					//��
		topLeft,				//����
		top,					//��
		topRight,				//�E��
		right,					//�E
		bottomRight,			//�E��
		bottom,					//��
		bottomLeft,				//����
	};

	struct MasuState {
		VECTOR centerPos;		//���S���W
		MasuMode masuMode;		//���̃��[�h
		int x;					//2�����z���X
		int z;					//2�����z���Y
	};

	struct Score {
		int moveCost;			//�ړ��R�X�g
		int estimationCost;		//����R�X�g
		int score;				//�v���C���[��ǐՂ��邤���œ��_��t����
		int currentIndex;		//�ړ���̃C���f�b�N�X
		int destinationIndex;	//�ړ���̃C���f�b�N�X
		Direction dir;			//���p
	};

	struct DesinationState {
		int index;
		int score;
	};

public:
	Aster(VECTOR pos);
	virtual ~Aster();

	//������
	void Init();

	//�X�V
	void Update();
	//�`��
	void Draw();

	/// <summary>
	/// �|�W�V�����������ɔz��̉��Ԗڂɑ��݂��邩�擾����
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	/// <param name="pos">�|�W�V����</param>
	void LocationInformation(VECTOR playerPos,VECTOR enemyPos);

	/// <summary>
	/// �o�H�T��
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// ���͂̏������݂��邩�T��
	/// </summary>
	void SearchAroundSquares();

	/// <summary>
	/// ���͂̏�����������
	/// </summary>
	void SearchSurrroundingSquares(bool skipCheckLeft, bool skipCheckRight, bool skipCheckTop, bool skipCheckBottom);

	/// <summary>
	/// ���̃X�R�A���擾����
	/// </summary>
	/// <param name="direction">����</param>
	/// <param name="index">���̔ԍ�</param>
	void ScoreCaluculation(Direction direction,int index);

	/// <summary>
	/// ���W���珡�̃C���f�b�N�X���擾����
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	int SearchCurrentIndex(VECTOR pos);

	/// <summary>
	/// �ړI�n�̍��W���擾����
	/// </summary>
	/// <returns></returns>
	VECTOR GetDestinationCoordinates(VECTOR playerPos, VECTOR enemyPos);


	bool SearchBlockadeMode(VECTOR pos);
private:

	int enemyIndex_ = 0;
	int playerIndex_ = 0;
	int moveCount_ = 0;

	int tempIndex_ = 0;

	int count_ = 0;

	std::unordered_map<int, MasuState> masu_;					//�ړ��͈�
	std::unordered_map<int, Score> scoreTable_;					//�ړ��͈͂̏��ɃX�R�A������
	std::unordered_map<int, Score> debugScoreTable;				//�f�o�b�O�p�̃X�R�A�e�[�u��(�폜���Ă悢)

	std::unordered_map<int, std::list<int>> preteriteIndex_;

	std::list<int> route_;

};

