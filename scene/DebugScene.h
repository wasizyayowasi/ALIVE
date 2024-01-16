#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <list>
#include <string>

class DebugScene : public SceneBase
{
public:

	DebugScene(SceneManager& manager);
	virtual ~DebugScene();

	//������
	void Init();
	//�폜
	void End();

	//�X�V
	void Update();
	//�`��
	void Draw();

private:

	int selectNum_ = 0;

	std::list<std::string> sceneName_;
};

