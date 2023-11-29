#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void ChangeScene(std::shared_ptr<SceneBase> scene);				//�V�[���J��
	void PushFrontScene(std::shared_ptr<SceneBase> scene);			//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
	void PushBackScene(std::shared_ptr<SceneBase> scene);			//���C���̃V�[����ۂ����܂ܕʂ̃V�[�������ʂɕ\������
	void SwapScene(std::shared_ptr<SceneBase> scene);				//�V�[��������ւ���
	void PopFrontScene();												//�ЂƂO�̃V�[���ɖ߂�
	void Update(const InputState& input);							//�V�[���̍X�V
	void Draw();													//�V�[���̕`��
	bool End() { return isEnd_; };									//���C��while�����I�������邽��
	void SetEndFlag(bool flag) { isEnd_ = flag; }
private:
	std::deque<std::shared_ptr<SceneBase>> scenes_;					//�V�[���{��

	bool isEnd_ = false;

	int debugDrawTime_ = 0;
	int debugUpdateTime_ = 0;

};

