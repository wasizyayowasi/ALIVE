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
	void init();
	//�폜�p�֐�
	void end();

	//�X�V
	void update(const InputState& input);
	//�`��
	void draw();

	//���݂�key���͏���`��
	void keyStateDraw();
	//key�̖��O��`��
	void keyNameDraw();
	//key�̉摜��`��
	void keyGraphDraw();

	//�ύX������key����͂����邽�߂̃|�b�v�A�b�v�`��
	void changeKeyPopUpText();

	//�ǂ̓��͑��u�̃L�[��ύX���邩��I������
	void selectChangeKeyUpdate();
	//�ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
	void changeKeyUpdate();

	//�`�������鍡�̂Ƃ���
	int getKeyName(int num);

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

