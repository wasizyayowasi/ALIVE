#include "SceneTitle1.h"

#include "GameMain.h"
#include "ScenePause.h"
#include "SceneManager.h"

#include "../object/ObjectManager.h"
#include "../object/Camera.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/UIItemManager.h"

#include <DxLib.h>
#include <algorithm>

namespace {
	const char* const player_model_Filename = "data/player/player16.mv1";
}

SceneTitle1::SceneTitle1(SceneManager& manager): SceneBase(manager),updateFunc_(&SceneTitle1::OpeningFadeInUpdate),drawFunc_(&SceneTitle1::OpeningDraw)
{
}

SceneTitle1::~SceneTitle1()
{
}

void SceneTitle1::Init()
{
	//インスタンス化
	model_ = std::make_shared<Model>(player_model_Filename);
	UI_ = std::make_shared<UIItemManager>();
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(0, 140, -370));

	objManager_->OpeningStageObjectGenerator();
	camera_->Init(VGet(0,140,0));

	//仮でライト処理を消している
	SetUseLighting(false);

	//UI文字列の作成
	menuName_.push_back("ニューゲーム");
	menuName_.push_back("ゲームを再開する");
	menuName_.push_back("設定");
	menuName_.push_back("終了");

	//UI画像の作成
	int font = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	float y = 120.0f;
	for (auto& menu : menuName_) {
		UI_->AddMenu(Game::screen_width / 2, Game::screen_height / 2 + y, 320, 100, menu.c_str(), font);
		y += 40.0f;
	}
}

void SceneTitle1::End()
{
}

void SceneTitle1::Update()
{
	(this->*updateFunc_)();
}

void SceneTitle1::Draw()
{
	(this->*drawFunc_)();
}

void SceneTitle1::FadeInUpdate()
{
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle1::NormalUpdate;
		return;
	}
}

void SceneTitle1::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//選択
	if (input.IsTriggered(InputType::up)) {
		selectNum_ = (std::max)(selectNum_ - 1, 0);
	}
	if (input.IsTriggered(InputType::down)) {
		selectNum_ = (std::min)(selectNum_ + 1, static_cast<int>(menuName_.size() - 1));
	}

	//決定
	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &SceneTitle1::FadeOutUpdate;
	}
}

void SceneTitle1::OpeningSoundUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();
	int notPlaying = static_cast<int>(SoundPlay::notPlaying);

	if (sound.CheckSoundFile("alarm") == notPlaying) {
		if (sound.CheckSoundFile("stopAlarm") == notPlaying) {
			sound.Play("stopAlarm");
			drawFunc_ = &SceneTitle1::OpeningDraw;
			updateFunc_ = &SceneTitle1::OpeningFadeInUpdate;
			fadeValue_ = 255;
			fadeTimer_ = fadeInterval_ * 4;
		}
	}
}

void SceneTitle1::OpeningFadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		fadeValue_ = 0;
		return;
	}

	camera_->ChangeOfFocus();

}

void SceneTitle1::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		SceneChange();
		fadeValue_ = 0;
		return;
	}
}

void SceneTitle1::NormalDraw()
{
	//UIの描画
	UI_->AlphaChangeDraw(selectNum_, fadeValue_);
}

void SceneTitle1::OpeningDraw()
{
	
	DrawString(0, 0, "aaaaaaaaaaaaaaaaaaa", 0xffffff);

	objManager_->Draw({0,0,0});

	camera_->tempdraw();

	model_->Draw();

	DrawSphere3D({ 0,140,0 }, 32, 32, 0xff0000, 0xff0000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//背景
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void SceneTitle1::SceneChange()
{
	switch (selectNum_) {
	case 0:
		ExternalFile::GetInstance().ClearSaveData();
		updateFunc_ = &SceneTitle1::OpeningSoundUpdate;
		SoundManager::GetInstance().Play("alarm");
		break;
	case 1:
		ExternalFile::GetInstance().LoadSaveData();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		break;
	case 2:
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}
