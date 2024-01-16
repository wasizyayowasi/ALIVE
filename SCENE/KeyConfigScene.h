#pragma once
#include "SceneBase.h"
#include <memory>
#include <map>

class UIItemManager;

class KeyConfigScene : public SceneBase
{
private:

	enum class Key
	{
		ESC,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,
		Key_0,
		Key_equal,

		Back,
		Tab,
		Q, W, E, R, T, Y, U, I, O, P,
		Left_parenthesis_key,
		Right_parenthesis_key,
		Big_enter,
		L_ctrl,
		A, S, D, F, G, H, J, K, L,
		Semicolon,
		L_shift,
		Backslash,

		Z,X,C,V,B,N,M,
		Comma,
		Period,
		Slash,
		R_shift,
		Asterisk,
		L_alt,
		Space,
		CaspLock,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		NumLock,
		ScrollLock,
		Key10_7,
		Key10_8,
		Key10_9,
		Key10_minus,
		Key10_4,
		Key10_5,
		Key10_6,
		Key10_plus,
		Key10_1,
		Key10_2,
		Key10_3,
		Key10_0,
		Key10_period,
		F11,
		F12,
		�J�i,
		�ϊ�,
		���ϊ�,
		YEN_mark,
		Caret,
		At_mark,
		Colon,
		����,
		Key10_enter,
		R_ctrl,
		Key10_slash,
		Print,
		Ralt,
		Pause,
		Home,
		Up_Arrow,
		Page_up,
		Left_Arrow,
		Right_Arrow,
		END,
		Down_Arrow,
		Page_Down,
		Insert,
		Delete,
		L_win,
		Rwin,
		App,
	};

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">manager���Q�Ƃ��A�V�[���x�[�X�Ɉ����Ƃ��ēn��</param>
	/// <param name="input">input�̏����Q�Ƃ���</param>
	KeyConfigScene(SceneManager& manager, const InputState& input);
	//�f�X�g���N�^
	virtual ~KeyConfigScene();

	//�������p�֐�
	void Init();
	//�폜�p�֐�
	void End();

	//�X�V
	void Update(const InputState& input);
	//�`��
	void Draw();

	//���݂�key���͏���`��
	void KeyStateDraw();
	
	//key�̉摜��`��
	void KeyGraphDraw();

	//�R���g���[���[���g�p����Ă���ꍇ�̕`��
	void ControllerDraw();

	//�ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
	void ChangeKeyPopUpText();

	//�ǂ̓��͑��u�̃L�[��ύX���邩��I������
	void SelectChangeKeyUpdate();
	//�ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
	void ChangeKeyborardUpdate();
	//�R���g���[���[�̏ꍇ�̍X�V
	void ControllerUpdate();

private:

	//������������Ȃ�
	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

private:

	int controllerHandle_ = -1;				//�R���g���[���[�摜
	int fontHandleSize21_ = -1;				//�t�H���g16�T�C�Y��ۊǂ���ϐ�
	int fontHandleSize42_ = -1;				//�t�H���g32�T�C�Y��ۊǂ���ϐ�
	int makeScreenHandle_ = -1;				//�쐬�����n���h����ۊǂ���ϐ�
	int keyTypeHandle_ = -1;				//keyType��`�悷�邽�߂̃O���t���󂯎�邽�߂�handle
	int textColor_ = 0xffffff;				//�J���[
	int selectNum_ = 0;						//���݂̑I��ԍ�

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	bool isEditing_ = false;							//�ҏW���t���O

	const InputState& inputState_;						//�R���X�g���N�^�̈���input�Q�Ƃ��󂯎��

	std::shared_ptr<UIItemManager> KeyUI_;				//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<UIItemManager> PadUI_;				//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

	std::map<int, Key> keyNum_;

	void (KeyConfigScene::* updateFunc_)(const InputState& input);
	void (KeyConfigScene::* changeKeyUpdateFunc_)();	//�����o�֐��|�C���^
	void (KeyConfigScene::* drawFunc_)();				//�����o�֐��|�C���^

};

