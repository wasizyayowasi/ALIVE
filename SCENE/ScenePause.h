#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <memory>
#include <vector>
#include <string>

class UIItemManager;

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

	int selectNum_ = 0;		//�I��ԍ�

	std::shared_ptr<UIItemManager> UI_;		//UI�}�l�[�W���̃X�}�[�g�|�C���^
	std::vector<std::string> menuName_;		//�|�[�Y���j���[�̖��O�p
};

