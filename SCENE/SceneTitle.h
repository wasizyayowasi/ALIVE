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

	void Update();
	void Draw();


private:

	void SceneChange();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

private:
	int fontHandle_ = -1;
	int titleWidth = 0;


	static constexpr int fadeInterval_ = 30;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	int titleHandle_ = -1;					//�^�C�g���摜�̕ۊǕϐ�
	int selectNum_ = 0;						//�I��

	std::vector<std::string> menuName_;		//���j���[�̕�����
	std::shared_ptr<UIItemManager> UI_;		//UI�}�l�[�W���[�̃X�}�[�g�|�C���^

	void (SceneTitle::* updateFunc_)();
};

