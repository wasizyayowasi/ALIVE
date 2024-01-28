#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <string>
#include <vector>
#include <memory>
#include <map>

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
	void SceneTitleFadeOutUpdate();

	/// <summary>
	/// �V�[����؂�ւ���
	/// </summary>
	void SceneChange();

	/// <summary>
	/// �J�����̐ݒ�
	/// </summary>
	void CameraSetting();

private:

	static constexpr int fadeInterval_ = 30;	//�C���^�[�o��
	int fadeTimer_ = 0;							//�t�F�[�h���鎞��
	int fadeValue_ = 0;							//�t�F�[�h�̓��ߒl
	int UIfadeValue_ = 0;						//UI�̃t�F�[�h���ߒl
	int fadeColor_ = 0x000000;					//��

	int titleHandle_ = -1;						//�^�C�g���摜�̕ۊǕϐ�
	int selectNum_ = 1;							//�I��

	std::vector<std::string> menuName_;			//���j���[�̕�����
	std::map<std::string, VECTOR> menuDrawPos_;	//���j���[��`�悷����W
	std::shared_ptr<Camera> camera_;			//�J�����̃X�p�[�g�|�C���^
	std::shared_ptr<UIItemManager> UI_;			//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<ObjectManager> objManager_;	//�I�u�W�F�N�g�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<Model> playerModel_;		//�v���C���[�̃X�}�[�g�|�C���^

	void (SceneTitle::* updateFunc_)();
};

