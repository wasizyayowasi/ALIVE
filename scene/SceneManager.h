#pragma once
#include <deque>
#include <memory>

class SceneBase;
class InputState;

class SceneManager
{
public:
	//�V�[���J��
	void ChangeScene(std::shared_ptr<SceneBase> scene);
	//���C���̃V�[����ۂ����܂ܕʂ̃V�[����O��ʂɕ\������
	void PushFrontScene(std::shared_ptr<SceneBase> scene);
	//���C���̃V�[����ۂ����܂ܕʂ̃V�[�������ʂɕ\������
	void PushBackScene(std::shared_ptr<SceneBase> scene);
	//�V�[��������ւ���
	void SwapScene(std::shared_ptr<SceneBase> scene);				
	//�ЂƂO�̃V�[���ɖ߂�
	void PopFrontScene();											
	//�V�[���̍X�V
//	void Update(const InputState& input);							
	void Update();							
	//�V�[���̕`��
	void Draw();													
	//���C��while�����I�������邽��
	bool End() { return isEnd_; };		
	//end�t���O��ύX����
	void SetEndFlag(bool flag) { isEnd_ = flag; }
	//�E�B���h�E���[�h��ύX����
	void ChangeWindowMode(bool windowMode);
	//�E�B���h�E���[�h�̎擾
	bool GetWindowMode() { return windowMode_; }

private:

	std::deque<std::shared_ptr<SceneBase>> scenes_;					//�V�[���{��

	bool isEnd_ = false;

	int debugDrawTime_ = 0;
	int debugUpdateTime_ = 0;

	bool windowMode_ = true;

};

