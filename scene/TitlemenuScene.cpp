#include "TitlemenuScene.h"
#include "SceneManager.h"
#include "ScenePause.h"
#include "GameMain.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/InputState.h"
#include "../util/UIItemManager.h"
#include "../util/ExternalFile.h"
#include <algorithm>

using namespace std;

TitlemenuScene::TitlemenuScene(SceneManager& manager):SceneBase(manager)
{
	
}

TitlemenuScene::~TitlemenuScene()
{
}

//初期化
void TitlemenuScene::Init()
{
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
	int y = 120;
	for (auto& menu : menuName_) {
		UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 2 + y,320, 100, menu.c_str(), font);
		y += 40;
	}

}

void TitlemenuScene::End()
{
	DeleteGraph(titleHandle_);
}

//更新
void TitlemenuScene::Update(const InputState& input)
{
	//選択
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//決定
	if (input.IsTriggered(InputType::space)) {
		SceneChange();
	}

}

//描画
void TitlemenuScene::Draw()
{
	//UIの描画
	UI_->AlphaChangeDraw(selectNum_);
	//タイトルの描画
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, titleHandle_, true);
}

//シーンの変更
void TitlemenuScene::SceneChange()
{
	switch (selectNum_) {
	case 0:
		ExternalFile::GetInstance().ClearSaveData();
		ExternalFile::GetInstance().LoadFile(false);
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		break;
	case 1:
		ExternalFile::GetInstance().LoadFile(true);
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
