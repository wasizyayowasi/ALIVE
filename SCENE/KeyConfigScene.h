#pragma once
#include "SceneBase.h"

enum class KeySelect {
	next,			/// ���֍s���{�^��
	prev,			/// �O�ɖ߂�{�^��
	pause,			/// �|�[�Y�{�^��
	up,				///��
	down,			///��
	left,			///��
	right,			///�E
	space,			///space
	z,				//���S����
	shift,			//shift
	ctrl,			///ctrl
};

class KeyConfigScene : public SceneBase
{
public:

	KeyConfigScene(SceneManager& manager, const InputState& input);
	virtual ~KeyConfigScene();

	void update(const InputState& input);
	void draw();

	void selectChangeKeyUpdate(const InputState& input);
	void changeKeyUpdate(const InputState& input);

private:

	int color_ = 0xff0000;		//�J���[
	int selectNum_ = 0;			//���݂̑I��ԍ�

	bool isEditing_ = false;	//�ҏW���t���O

	const InputState& inputState_;

	void (KeyConfigScene::* updateFunc_)(const InputState& input);

};

