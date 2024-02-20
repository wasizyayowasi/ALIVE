#pragma once
#include <map>
#include <DxLib.h>

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
	
	int fontPigumo42_ = -1;								//�t�H���g�n���h��

	int nextDisplayKeyType_ = 0;						//InputType��int�^�ɕϊ���������
	int nextDisplayBottanType_ = 0;						//XboxBottan��int�^�ɕϊ���������

	std::map<int, bool> pushBottan_;					//�����ꂽ�{�^��

	std::map<UIGraph, std::pair<float, float>> UIPos_;	//UI�̉�ʃ|�W�V���� pair�̒��g�bfirst�FX�bsecond�FY

	void(Tutorial::* drawFunc_)(InputState& input);

};

