#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"
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
	menuName_.push_back("音調整");
	menuName_.push_back("操作設定");
	menuName_.push_back("戻る");
	menuName_.push_back("タイトルへ");
	menuName_.push_back("デバッグシーンへ");

	//UI画像の作成
	int font = FontsManager::getInstance().GetFontHandle("ピグモ 0042");
	int y = 50;
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
			selectNum_ = (std::min)(selectNum_ + 1, 4);
		}
	}
	
	if (input.IsTriggered(InputType::space)) {
		 switch(selectNum_) {
		//サウンドセッティングへの遷移
		 case 0:
			 manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<SoundSettingScene>(manager_)));
			 break;
		//キーコンフィグシーンへの遷移
		case 1:
			manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_,input)));
			break;
		//一個前のシーン(メインシーン)へ遷移
		case 2:
			manager_.PopScene();
			break;
		//タイトルシーンへの遷移
		case 3:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//デバッグシーンに戻る
		case 4:
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		}
	}

	//一個前のシーン(メインシーン)へ遷移
	if (input.IsTriggered(InputType::pause)) {
		manager_.PopScene();
	}
}

void ScenePause::Draw()
{
	//背景黒の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//UIの描画
	UI_->Draw(selectNum_);

}
