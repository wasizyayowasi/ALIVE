#include "SceneTitle.h"
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
	const char* const enemy_model_Filename = "data/player/mv1/player.mv1";
}

SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//インスタンス化
	playerModel_ = std::make_shared<Model>(enemy_model_Filename);
	UI_ = std::make_shared<UIItemManager>();
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(0, 140, -370));

	//モデルの配置データをセットする
	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("title", "Player");
	playerModel_->SetScale(info.scale);
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::wakeUp2), false, true);

	//オブジェクトの生成
	objManager_->OpeningStageObjectGenerator();

	//カメラの初期化
	camera_->Init(VGet(0, 140, 0));

	//仮でライト処理を消している
	//SetUseLighting(false);

	//タイトル画像の読み込み
	titleHandle_ = LoadGraph("data/graph/title.png");

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

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;
	drawFunc_ = &SceneTitle::UIDraw;

	int fadeTimer_ = 0;
	int fadeValue_ = 0;
}

void SceneTitle::End()
{
}

void SceneTitle::Update()
{
	(this->*updateFunc_)();
}

void SceneTitle::Draw()
{
	(this->*drawFunc_)();
}

void SceneTitle::FadeInUpdate()
{
	//フェードイン
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	fadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle::UIUpdate;
		return;
	}
}

void SceneTitle::UIUpdate()
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
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}
}

void SceneTitle::OpeningSoundUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();
	int notPlaying = static_cast<int>(SoundPlay::notPlaying);

	//サウンドが鳴り終わったら
	if (sound.CheckSoundFile("alarm") == notPlaying) {
		if (sound.CheckSoundFile("stopAlarm") == notPlaying) {
			sound.Play("stopAlarm");
			drawFunc_ = &SceneTitle::OpeningDraw;
			updateFunc_ = &SceneTitle::OpeningFadeInUpdate;
			fadeValue_ = 255;
			fadeTimer_ = fadeInterval_ * 4;
		}
	}
}

void SceneTitle::OpeningFadeInUpdate()
{
	//カメラの更新
	camera_->OpeningCameraUpdate();

	//フェードイン
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		fadeValue_ = 0;
		updateFunc_ = &SceneTitle::OpeningUpdate;
		return;
	}
}

void SceneTitle::OpeningUpdate()
{
	//モデルの描画
	playerModel_->Update();

	//カメラの更新
	camera_->OpeningCameraUpdate();

	//アニメーションが終了次第
	if (playerModel_->IsAnimEnd()) {
		updateFunc_ = &SceneTitle::OpeningFadeOutUpdate;
	}

}

void SceneTitle::UIFadeOutUpdate()
{
	//フェードアウト
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		SceneChange();
		fadeValue_ = 0;
		return;
	}
}

void SceneTitle::OpeningFadeOutUpdate()
{
	//フェードアウト
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

void SceneTitle::UIDraw()
{
	//UIの描画
	UI_->AlphaChangeDraw(selectNum_, fadeValue_);

	//fadeValue_の値によって透過具合が変化するタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawRotaGraph(Game::screen_width / 2, Game::screen_height / 3, 1.0f, 0.0f, titleHandle_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneTitle::OpeningDraw()
{
	//オブジェクトの描画
	objManager_->Draw({0,0,0});

	//モデルの描画
	playerModel_->Draw();


	//fadeValue_の値によって透過具合が変化する背景の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

}

void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		ExternalFile::GetInstance().ClearSaveData();
		updateFunc_ = &SceneTitle::OpeningSoundUpdate;
		SoundManager::GetInstance().Play("alarm");
		break;
	case 1:
		ExternalFile::GetInstance().LoadSaveData();
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		break;
	case 2:
		Init();
		manager_.SwapScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}
