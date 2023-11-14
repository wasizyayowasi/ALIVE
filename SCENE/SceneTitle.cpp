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

	//タイトル画像の読み込み
	titleHandle_ = LoadGraph("data/graph/title.png");
	//インスタンス化
	UI_ = std::make_shared<UIItemManager>();

	//UI文字列の作成
	menuName_.push_back("ニューゲーム");
	menuName_.push_back("ゲームを再開する");
	menuName_.push_back("設定");
	menuName_.push_back("終了");

	//UI画像の作成
	int font = FontsManager::getInstance().GetFontHandle("ピグモ 0042");
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
	//UIの描画
	UI_->AlphaChangeDraw(selectNum_,fadeValue_);
	//タイトルの描画
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

	//選択
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//決定
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

//シーンの変更
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
