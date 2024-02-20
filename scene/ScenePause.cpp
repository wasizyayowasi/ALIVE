#include "ScenePause.h"
#include "SceneTitle.h"
#include "DebugScene.h"
#include "SceneManager.h"
#include "SettingScene.h"
#include "KeyConfigScene.h"

#include "../util/Util.h"
#include "../util/game.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

//コンストラクタ
ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

//デストラクタ
ScenePause::~ScenePause()
{
}

//初期化
void ScenePause::Init()
{
	//インスタンス化
	UI_ = std::make_shared<UIItemManager>();

	//UI文字列の作成
	menuName_.push_back("戻る");
	menuName_.push_back("設定");
	menuName_.push_back("タイトルへ");
#ifdef _DEBUG
	menuName_.push_back("デバッグシーンへ");
#endif // _DEBUG

	//UI画像の作成
	int font = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	int y = 100;
#ifdef _DEBUG
#else
	y += 40;
#endif // _DEBUG

	for (auto& menu : menuName_) {
		UI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 2 + y), 640, 200, menu.c_str(), font);
		y += 40;
	}
}

//終了
void ScenePause::End()
{
}

//更新
void ScenePause::Update()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//TODO:まとめる
	//項目選択
	{
		if (input.IsTriggered(InputType::Up)) {
			selectNum_ = (std::max)(selectNum_ - 1, 0);
		}
		if (input.IsTriggered(InputType::Down)) {
			selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size()) - 1);
		}
	}
	
	if (input.IsTriggered(InputType::Space)) {
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
	if (input.IsTriggered(InputType::Pause)) {
		manager_.PopFrontScene();
	}
}

//描画
void ScenePause::Draw()
{
	//背景黒の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);

	//タイトル画像の描画
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, GraphManager::GetInstance().GetGraph("title"), true);

	//UIの描画
	UI_->AlphaChangeDraw(selectNum_,255);
}
