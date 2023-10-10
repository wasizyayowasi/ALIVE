#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	void ChangeScene(std::shared_ptr<SceneBase> manager);			//�V�[���J��
	void PushScene(std::shared_ptr<SceneBase> manager);			//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
	void SwapScene(std::shared_ptr<SceneBase> manager);			//�V�[��������ւ���
	void PopScene();							//�ЂƂO�̃V�[���ɖ߂�
	void Update(const InputState& input);		//�V�[���̍X�V
	void Draw();								//�V�[���̕`��
	bool End() { return isEnd_; };				//���C��while�����I�������邽��
	void SetEndFlag(bool flag) { isEnd_ = flag; }
private:
	std::deque<std::shared_ptr<SceneBase>> scenes_;				//�V�[���{��

	bool isEnd_ = false;

};

