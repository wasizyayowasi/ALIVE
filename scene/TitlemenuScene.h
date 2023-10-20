#pragma once
#include "SceneBase.h"
#include <vector>
#include <string>
#include <memory>

class InputState;
class UIItemManager;

class TitlemenuScene : public SceneBase
{
public:
	TitlemenuScene(SceneManager& manager);
	virtual ~TitlemenuScene();

	//������
	void Init();
	//�I��
	void End();

	//�X�V
	void Update(const InputState& input);
	//�`��
	void Draw();

	//�V�[���̕ύX
	void SceneChange();

private:

	int titleHandle_ = -1;					//�^�C�g���摜�̕ۊǕϐ�
	int selectNum_ = 0;						//�I��

	std::vector<std::string> menuName_;		//���j���[�̕�����
	std::shared_ptr<UIItemManager> UI_;		//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

};

