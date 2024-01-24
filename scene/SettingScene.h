#pragma once
#include "SceneBase.h"
#include <memory>
#include <string>

class InputState;
class UIItemManager;

class SettingScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SettingScene(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SettingScene();

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �I��
	/// </summary>
	void End();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	void NormalUpdate();

	/// <summary>
	/// �K�E�X�������{�����t�F�[�h�C��
	/// </summary>
	void GaussFadeInUpdate();

	/// <summary>
	/// �K�E�X�������{�����t�F�[�h�A�E�g
	/// </summary>
	void GaussFadeOutUpdate();

	/// <summary>
	/// BGM�̉��ʂ�ύX����
	/// </summary>
	void BGMUpdate();

	/// <summary>
	/// SE�̉��ʂ�ύX����
	/// </summary>
	void SEUpdate();

	/// <summary>
	/// �E�B���h�E���[�h��ύX����
	/// </summary>
	void ChangeWindowUpdate();

	/// <summary>
	/// updateFunc�̒��g��ύX����
	/// </summary>
	void ChangeUpdateFunc();

private:

	int makeScreenHandle_ = -1;

	int volumeBGM_ = 0;
	int volumeSE_ = 0;

	int selectNum_ = 0;

	bool windowMode_ = true;					//�E�B���h�E���[�h

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	std::shared_ptr<SceneBase> nextScene_;

	std::string windowModeText_ = "��  �E�B���h�E���[�h  ��";

	std::shared_ptr<UIItemManager> UIManager_;

	void(SettingScene::* updateFunc_)();

};

