#pragma once
#include "KeyData.h"
#include <DxLib.h>
#include <map>

class Tutorial
{
private:
	//UI�摜�̗�
	enum class UIGraph {
		XboxBotton,
		KeyBord,
		max,
	};
	//pad�{�^���̗�
	enum class XboxBotton {
		A,
		B,
		X,
		Y,
		max,
	};
	//�`���[�g���A��
	enum class TutorialGimmick {
		Switch,
		Lever,
		Crank,
		max,
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Tutorial();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Tutorial();

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="pos">�v���C���[�̃|�W�V����</param>
	void Update(VECTOR playerPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �L�[�̉摜��`�悷��
	/// </summary>
	/// <param name="num">�L�[�̔ԍ�</param>
	void KeyGraphDraw(int keyNum);

	/// <summary>
	/// �����`�悵�Ȃ�
	/// </summary>
	void NoneDraw();

	/// <summary>
	/// �X�C�b�`�̃`���[�g���A��
	/// </summary>
	void SwitchTutorialDraw();

	/// <summary>
	/// �N�����N�̃`���[�g���A��
	/// </summary>
	void CranckTutorialDraw();

	/// <summary>
	/// ���铮��̃`���[�g���A��
	/// </summary>
	void RunTutorialDraw();

	/// <summary>
	/// �W�����v�̃`���[�g���A��
	/// </summary>
	void JumpTutorialDraw();

	/// <summary>
	/// �G���x�[�^�[�̃`���[�g���A��
	/// </summary>
	void ElevatorTutorialDraw();

private:
	
	std::map<UIGraph,int> UIHandle_;					//�摜
	std::map<int, Key> keyNum_;							//�L�[�ԍ�
	std::map<UIGraph, std::pair<float, float>> UIPos_;	//UI�̉�ʃ|�W�V���� pair�̒��g�bfirst�FX�bsecond�FY

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)();

};

