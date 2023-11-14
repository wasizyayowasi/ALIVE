#include "SceneTitle.h"
#include "SceneManager.h"
#include "GameMain.h"
#include "ScenePause.h"

#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "../util/ExternalFile.h"

#include <DxLib.h>
#include <algorithm>



SceneTitle::SceneTitle(SceneManager& manager) : SceneBase(manager), updateFunc_(&SceneTitle::fadeInUpdate)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{

	manager_.PushBackScene(std::shared_ptr(std::make_shared<GameMain>(manager_)));

	//�^�C�g���摜�̓ǂݍ���
	titleHandle_ = LoadGraph("data/graph/title.png");
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//UI������̍쐬
	menuName_.push_back("�j���[�Q�[��");
	menuName_.push_back("�Q�[�����ĊJ����");
	menuName_.push_back("�ݒ�");
	menuName_.push_back("�I��");

	//UI�摜�̍쐬
	int font = FontsManager::getInstance().GetFontHandle("�s�O�� 0042");
	float y = 120.0f;
	for (auto& menu : menuName_) {
		UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 2 + y, 320, 100, menu.c_str(), font);
		y += 40.0f;
	}
}

void SceneTitle::End()
{
	DeleteGraph(titleHandle_);
}

void SceneTitle::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

void SceneTitle::Draw()
{

	DrawString(0, 0, "SceneTitle", 0xffffff);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//UI�̕`��
	UI_->AlphaChangeDraw(selectNum_,fadeValue_);
	//�^�C�g���̕`��
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, titleHandle_, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::fadeInUpdate(const InputState& input)
{
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	fadeValue_ = static_cast <int>(255 * timer);
	if (fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle::normalUpdate;
		return;
	}

	fadeTimer_++;

}

void SceneTitle::normalUpdate(const InputState& input)
{		

	//�I��
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//����
	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &SceneTitle::fadeOutUpdate;
	}

}

void SceneTitle::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		SceneChange();
		fadeValue_ = 0;
		return;
	}
}

//�V�[���̕ύX
void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		ExternalFile::GetInstance().ClearSaveData();
		ExternalFile::GetInstance().LoadFile(false);
		manager_.PopFrontScene();
		break;
	case 1:
		ExternalFile::GetInstance().LoadFile(true);
		manager_.PopFrontScene();
		break;
	case 2:
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		DxLib_End();
		break;
	}
}
