#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>
#include <string>

class ScenePause : public SceneBase
{
public:
	//�R���X�g���N�^
	ScenePause(SceneManager& manager);
	//�f�X�g���N�^
	virtual ~ScenePause();

	//�������p
	void init();
	//�폜�p
	void end();

	//�X�V
	void update(const InputState& input);
	//�`��
	void draw();

private:

	int selectionNum_ = 0;		//�I��ԍ�

	bool isDecision_ = false;

	std::vector<std::string> menuName_;		//�|�[�Y���j���[�̖��O�p
};

