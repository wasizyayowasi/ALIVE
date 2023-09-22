#include "ScenePause.h"
#include "SceneManager.h"
#include "SceneTitle.h"
#include "KeyConfigScene.h"
#include "SoundSettingScene.h"
#include "DebugScene.h"
#include "../util/InputState.h"
#include "../util/game.h"
#include <algorithm>

ScenePause::ScenePause(SceneManager& manager):SceneBase(manager)
{
}

ScenePause::~ScenePause()
{
}

void ScenePause::update(const InputState& input)
{
	//項目選択
	{
		if (input.isTriggered(InputType::up)) {
			selectionNum_ = (std::max)(selectionNum_ - 1, 0);
		}
		if (input.isTriggered(InputType::down)) {
			selectionNum_ = (std::min)(selectionNum_ + 1, 4);
		}
	}
	

	if (input.isTriggered(InputType::next)) {
		 switch(selectionNum_) {
		//サウンドセッティングへの遷移
		 case 0:
			 manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<SoundSettingScene>(manager_)));
			 break;
		//キーコンフィグシーンへの遷移
		case 1:
			manager_.swapScene(std::shared_ptr<SceneBase>(std::make_shared<KeyConfigScene>(manager_,input)));
			break;
		//一個前のシーン(メインシーン)へ遷移
		case 2:
			manager_.popScene();
			break;
		//タイトルシーンへの遷移
		case 3:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			break;
		//デバッグシーンに戻る
		case 4:
			manager_.changeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
			break;
		}
	}

	//一個前のシーン(メインシーン)へ遷移
	if (input.isTriggered(InputType::pause)) {
		manager_.popScene();
	}
}

void ScenePause::draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawString(0, 0, "scenePause",0xff0000);

	DrawString(90, 16 * selectionNum_ + 32, "←", 0xffff00);

	DrawString(0, 32, "音調整",0x00ff00);
	DrawString(0, 48, "操作設定",0x00ff00);
	DrawString(0, 64, "戻る",0x00ff00);
	DrawString(0, 80, "タイトルへ",0x00ff00);
	DrawString(0, 96, "デバッグシーンへ",0x00ff00);
}
