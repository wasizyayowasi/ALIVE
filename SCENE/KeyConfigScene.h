#pragma once
#include "SceneBase.h"
#include <memory>

class UIItemManager;

class KeyConfigScene : public SceneBase
{
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

	//�ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
	void ChangeKeyPopUpText();

	//�ǂ̓��͑��u�̃L�[��ύX���邩��I������
	void SelectChangeKeyUpdate();
	//�ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
	void ChangeKeyUpdate();

	//�`�������鍡�̂Ƃ���
	int GetKeyName(int num);

private:

	//������������Ȃ�
	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

private:

	int fontHandleSize16_ = -1;				//�t�H���g16�T�C�Y��ۊǂ���ϐ�
	int fontHandleSize32_ = -1;				//�t�H���g32�T�C�Y��ۊǂ���ϐ�
	int makeScreenHandle_ = -1;				//�쐬�����n���h����ۊǂ���ϐ�
	int keyTypeHandle_ = -1;				//keyType��`�悷�邽�߂̃O���t���󂯎�邽�߂�handle
	int textColor_ = 0xffffff;				//�J���[
	int selectNum_ = 0;						//���݂̑I��ԍ�

	//������������������Ȃ�
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	bool isEditing_ = false;							//�ҏW���t���O

	const InputState& inputState_;						//�R���X�g���N�^�̈���input�Q�Ƃ��󂯎��

	std::shared_ptr<UIItemManager> UI_;					//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

	void (KeyConfigScene::* updateFunc_)(const InputState& input);
	void (KeyConfigScene::* changeKeyUpdateFunc_)();	//�����o�֐��|�C���^
	void (KeyConfigScene::* drawFunc_)();				//�����o�֐��|�C���^

};

