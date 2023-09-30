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

void ScenePause::init()
{
	menuName_.push_back("音調整");
	menuName_.push_back("操作設定");
	menuName_.push_back("戻る");
	menuName_.push_back("タイトルへ");
	menuName_.push_back("デバッグシーンへ");
}

void ScenePause::end()
{
}

void ScenePause::update(const InputState& input)
{
	//TODO:まとめる
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

	int y = 32;
	for (auto& menu : menuName_) {
		DrawFormatString(0, y, 0x00ff00, "%s", menu.c_str());
		y += 16;
	}
}
