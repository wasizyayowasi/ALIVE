#pragma once
#include <DxLib.h>
#include <map>

class InputState;

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
	/// �����`�悵�Ȃ�
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void NoneDraw(InputState& input);

	/// <summary>
	/// �X�C�b�`�̃`���[�g���A��
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void SwitchTutorialDraw(InputState& input);

	/// <summary>
	/// �N�����N�̃`���[�g���A��
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void CranckTutorialDraw(InputState& input);

	/// <summary>
	/// ���铮��̃`���[�g���A��
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void RunTutorialDraw(InputState& input);

	/// <summary>
	/// �W�����v�̃`���[�g���A��
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void JumpTutorialDraw(InputState& input);

	/// <summary>
	/// �G���x�[�^�[�̃`���[�g���A��
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void ElevatorTutorialDraw(InputState& input);

	/// <summary>
	/// ���̂̑���̃`���[�g���A��
	/// </summary>
	/// <param name="input">InputState�N���X�̎Q��</param>
	void CorpseScaffoldDraw(InputState& input);
private:
	
	int fontPigumo42_ = 0;

	static constexpr int fadeInterval_ = 5;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::map<UIGraph,int> UIHandle_;					//�摜
	std::map<UIGraph, std::pair<float, float>> UIPos_;	//UI�̉�ʃ|�W�V���� pair�̒��g�bfirst�FX�bsecond�FY

	VECTOR tutorialDrawPos_ = {};

	void(Tutorial::* drawFunc_)(InputState& input);

};

