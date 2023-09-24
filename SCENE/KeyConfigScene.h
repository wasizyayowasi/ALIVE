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

	//�X�V
	void update(const InputState& input);
	//�`��
	void draw();

	//�ǂ̓��͑��u�̃L�[��ύX���邩��I������
	void selectChangeKeyUpdate();
	//�ύX����L�[���ǂ̃L�[�ɕύX����̂������肷��
	void changeKeyUpdate();

	//�`�������鍡�̂Ƃ���
	int getKeyName(int num);

private:

	int aiu = 1;

	int keyTypeHandle_ = -1;	//keyType��`�悷�邽�߂̃O���t���󂯎�邽�߂�handle

	int textColor_ = 0xff0000;		//�J���[
	int selectNum_ = 0;			//���݂̑I��ԍ�

	bool isEditing_ = false;	//�ҏW���t���O

	const InputState& inputState_;				//�R���X�g���N�^�̈���input�Q�Ƃ��󂯎��

	void (KeyConfigScene::* updateFunc_)();		//�����o�֐��|�C���^

};

