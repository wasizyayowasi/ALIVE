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
	void Init();
	//�폜�p
	void End();

	//�X�V
	void Update(const InputState& input);
	//�`��
	void Draw();

private:

	int selectionNum_ = 0;		//�I��ԍ�

	bool isDecision_ = false;

	std::vector<std::string> menuName_;		//�|�[�Y���j���[�̖��O�p
};

