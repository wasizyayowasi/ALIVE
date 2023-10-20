#include "TitlemenuScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "GameMain.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/InputState.h"
#include "../util/UIItemManager.h"
#include "../util/LoadExternalFile.h"
#include <algorithm>

using namespace std;

TitlemenuScene::TitlemenuScene(SceneManager& manager):SceneBase(manager)
{
	
}

TitlemenuScene::~TitlemenuScene()
{
}

//������
void TitlemenuScene::Init()
{
	//�^�C�g���摜�̓ǂݍ���
	titleHandle_ = LoadGraph("data/graph/title.png");
	//�C���X�^���X��
	UI_ = std::make_shared<UIItemManager>();

	//UI������̍쐬
	menuName_.push_back("New Game");
	menuName_.push_back("Continue");
	menuName_.push_back("Setting");
	menuName_.push_back("end");

	//UI�摜�̍쐬
	int font = FontsManager::getInstance().GetFontHandle("High Tower Text32");
	int y = 120;
	for (auto& menu : menuName_) {
		UI_->addMenu(Game::screen_width / 2, Game::screen_height / 2 + y,320, 100, menu.c_str(), font);
		y += 40;
	}

}

void TitlemenuScene::End()
{
	DeleteGraph(titleHandle_);
}

//�X�V
void TitlemenuScene::Update(const InputState& input)
{
	//�I��
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//����
	if (input.IsTriggered(InputType::space)) {
		SceneChange();
	}

}

//�`��
void TitlemenuScene::Draw()
{
	//UI�̕`��
	UI_->draw(selectNum_);
	//�^�C�g���̕`��
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, titleHandle_, true);
}

//�V�[���̕ύX
void TitlemenuScene::SceneChange()
{
	switch (selectNum_) {
	case 0:
		LoadExternalFile::GetInstance().LoadSaveFile(false);
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		break;
	case 1:
		LoadExternalFile::GetInstance().LoadSaveFile(true);
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		break;
	case 2:
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		DxLib_End();
		break;
	}
}