#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void changeScene(std::shared_ptr<SceneBase> manager);			//�V�[���J��
	void pushScene(std::shared_ptr<SceneBase> manager);			//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
	void swapScene(std::shared_ptr<SceneBase> manager);			//�V�[��������ւ���
	void popScene();							//�ЂƂO�̃V�[���ɖ߂�
	void update(const InputState& input);		//�V�[���̍X�V
	void draw();								//�V�[���̕`��
private:
	std::deque<std::shared_ptr<SceneBase>> scenes_;				//�V�[���{��
};

