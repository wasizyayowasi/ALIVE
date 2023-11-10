#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyConfigScene.h"
#include "SettingScene.h"
#include "DebugScene.h"

#include "../util/InputState.h"
#include "../util/game.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "../util/Util.h"

#include <algorithm>

ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::Init()
{
	//インスタンス化
	UI_ = std::make_shared<UIItemManager>();

	//UI文字列の作成
	menuName_.push_back("再開");
	menuName_.push_back("設定");
	menuName_.push_back("タイトルへ");
	menuName_.push_back("デバッグシーンへ");

	//UI画像の作成
	int font = FontsManager::getInstance().GetFontHandle("ピグモ 0042");
	int y = 100;
	for (auto& menu : menuName_) {
		UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 2 + y, 640, 200, menu.c_str(), font);
		y += 40;
	}

}

void ScenePause::End()
{
}

void ScenePause::Update(const InputState& input)
{
	//TODO:まとめる
	//項目選択
	{
		if (input.IsTriggered(InputType::up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (input.IsTriggered(InputType::down)) {
			selectNum_ = (std::min)(selectNum_ + 1, 3);
		}
	}
	
	if (input.IsTriggered(InputType::space)) {
		 switch(selectNum_) {
		//一個前のシーン(メインシーン)へ遷移
		case 0:
			manager_.PopFrontScene();
			break;
		//サウンドセッティングへの遷移
		 case 1:
			 manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SettingScene>(manager_)));
			 break;
		//タイトルシーンへの遷移
		case 2:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//デバッグシーンに戻る
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		}
	}

	//一個前のシーン(メインシーン)へ遷移
	if (input.IsTriggered(InputType::pause)) {
		manager_.PopFrontScene();
	}
}

void ScenePause::Draw()
{
	//背景黒の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//UIの描画
	UI_->AlphaChangeDraw(selectNum_);

}
