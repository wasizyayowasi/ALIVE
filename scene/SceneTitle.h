#pragma once
#include "SceneBase.h"
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <DxLib.h>

class Model;
class Camera;
class LightBulb;
class UIItemManager;
class ObjectManager;

class SceneTitle : public SceneBase
{
private:
	//�v�@36byte
	struct CameraInfo {
		VECTOR targetPos;	//�ړI�̏ꏊ		//12byte
		VECTOR targetView;	//����ꏊ			//12byte
		VECTOR upVec;		//�J�����̏����	//12byte
	};
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
	/// ���C�g�̐ݒ�
	/// </summary>
	void LightSetting();

	/// <summary>
	/// �J�����̔z�u���̐ݒ�
	/// </summary>
	void CameraSettingPos();

	/// <summary>
	/// �I��ԍ��̍X�V
	/// </summary>
	void SelectNumUpdate();

	/// <summary>
	/// �t�F�[�h�C��
	/// </summary>
	void FadeInUpdate();

	/// <summary>
	/// �ʏ펞�̍X�V
	/// </summary>
	void UIUpdate();

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

private:

	static constexpr int fadeInterval_ = 30;	//�C���^�[�o��
	int fadeTimer_ = 0;							//�t�F�[�h���鎞��
	int fadeValue_ = 0;							//�t�F�[�h�̓��ߒl
	int UIfadeValue_ = 0;						//UI�̃t�F�[�h���ߒl
	int fadeColor_ = 0x000000;					//��

	int fontHandle_ = -1;
	int titleHandle_ = -1;						//�^�C�g���摜�̕ۊǕϐ�
	int selectNum_ = 1;							//�I��
	int oldSelectNum_ = 1;

	float outAngle_ = 0.0f;						//�X�|�b�g���C�g�̊O���̊p�x
	float inAngle_ = 0.0f;						//�X�|�b�g���C�g�̓����̊p�x

	std::shared_ptr<Camera> camera_;			//�J�����̃X�p�[�g�|�C���^
	std::shared_ptr<UIItemManager> UI_;			//UI�}�l�[�W���[�̃X�}�[�g�|�C���^
	std::shared_ptr<LightBulb> lightBulb_;		//�d��
	std::shared_ptr<Model> subPlayerModel_;		//�v���C���[�̃X�}�[�g�|�C���^
	std::shared_ptr<Model> mainPlayerModel_;	//�v���C���[�̃X�}�[�g�|�C���^
	std::shared_ptr<ObjectManager> objManager_;	//�I�u�W�F�N�g�}�l�[�W���[�̃X�}�[�g�|�C���^

	std::vector<std::string> menuName_;			//���j���[�̕�����
	std::vector<int> lightHandle_;				//���C�g�n���h��
	std::vector<VECTOR> lightDir_;				//���C�g�̃f�B���N�V����(����)
	std::vector<CameraInfo> cameraInfo_;		//�J�����̏��

	std::map<std::string, VECTOR> menuDrawPos_;	//���j���[��`�悷����W

	void (SceneTitle::* updateFunc_)();
};

