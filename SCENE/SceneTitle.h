#pragma once
#include "SceneBase.h"
#include<string>
#include <vector>
#include <memory>

class UIItemManager;
class ObjectManager;
class Camera;
class Model;

class SceneTitle : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="manager">�V�[���}�l�[�W���[�̎Q��</param>
	SceneTitle(SceneManager& manager);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~SceneTitle();

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

private:
	/// <summary>
	/// �t�F�[�h�C��
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	void UIUpdate();

	/// <summary>
	/// �I�[�v�j���O�T�E���h�̍X�V
	/// </summary>
	void OpeningSoundUpdate();

	/// <summary>
	/// �I�[�v�j���O�t�F�[�h�C���̕`��
	/// </summary>
	void OpeningFadeInUpdate();

	/// <summary>
	/// �I�[�v�j���O�̍X�V
	/// </summary>
	void OpeningUpdate();

	/// <summary>
	/// UI�̃t�F�[�h�A�E�g
	/// </summary>
	void UIFadeOutUpdate();

	/// <summary>
	/// �I�[�v�j���O�̃t�F�[�h�A�E�g
	/// </summary>
	void OpeningFadeOutUpdate();

	/// <summary>
	/// �ʏ펞�̕`��
	/// </summary>
	void UIDraw();

	/// <summary>
	/// �I�[�v�j���O�̕`��
	/// </summary>
	void OpeningDraw();

	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	void SceneChange();

private:
	int fontHandle_ = -1;
	int titleWidth = 0;

	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	int titleHandle_ = -1;						//�^�C�g���摜�̕ۊǕϐ�
	int selectNum_ = 0;							//�I��

	std::vector<std::string> menuName_;			//���j���[�̕�����
	std::shared_ptr<Camera> camera_;			//�J�����̃X�p�[�g�|�C���^
	std::shared_ptr<UIItemManager> UI_;			//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<ObjectManager> objManager_;	//�I�u�W�F�N�g�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<Model> playerModel_;		//�v���C���[�̃X�}�[�g�|�C���^

	void (SceneTitle::* updateFunc_)();
	void (SceneTitle::* drawFunc_)();
};

