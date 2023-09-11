#pragma once
#include <deque>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void changeScene(SceneBase* scene);			//�V�[���J��
	void pushScene(SceneBase* scene);			//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
	void swapScene(SceneBase* scene);			//�V�[��������ւ���
	void popScene();							//�ЂƂO�̃V�[���ɖ߂�
	void update(const InputState& input);		//�V�[���̍X�V
	void draw();								//�V�[���̕`��
private:
	std::deque<SceneBase*> scenes_;				//�V�[���{��
};

