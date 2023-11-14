#pragma once
#include "SceneBase.h"
#include<string>
#include <vector>
#include <memory>

class UIItemManager;

class SceneTitle : public SceneBase
{
public:
	SceneTitle(SceneManager& manager);
	virtual ~SceneTitle();

	void Init();
	void End();

	void Update(const InputState& input);
	void Draw();

	void SceneChange();

private:

	int fontHandle_ = -1;
	int titleWidth = 0;

	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	int titleHandle_ = -1;					//�^�C�g���摜�̕ۊǕϐ�
	int selectNum_ = 0;						//�I��

	std::vector<std::string> menuName_;		//���j���[�̕�����
	std::shared_ptr<UIItemManager> UI_;		//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

	void (SceneTitle::* updateFunc_)(const InputState& input);
};

