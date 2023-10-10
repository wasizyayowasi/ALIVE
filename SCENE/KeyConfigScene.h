#pragma once
#include "SceneBase.h"

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
	//key�̖��O��`��
	void KeyNameDraw();
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

	int fontHandle_ = -1;

	int makeScreenHandle_ = -1;

	int keyTypeHandle_ = -1;	//keyType��`�悷�邽�߂̃O���t���󂯎�邽�߂�handle

	int textColor_ = 0xffffff;		//�J���[
	int selectNum_ = 0;			//���݂̑I��ԍ�

	bool isEditing_ = false;	//�ҏW���t���O

	const InputState& inputState_;				//�R���X�g���N�^�̈���input�Q�Ƃ��󂯎��

	void (KeyConfigScene::* updateFunc_)();		//�����o�֐��|�C���^
	void (KeyConfigScene::* drawFunc_)();		//�����o�֐��|�C���^

};

