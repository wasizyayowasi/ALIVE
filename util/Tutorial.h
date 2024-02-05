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
	
	int fotnPigumo42_ = 0;

	static constexpr int fadeInterval_ = 5;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::map<UIGraph,int> UIHandle_;					//�摜
	std::map<UIGraph, std::pair<float, float>> UIPos_;	//UI�̉�ʃ|�W�V���� pair�̒��g�bfirst�FX�bsecond�FY

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)();

};

