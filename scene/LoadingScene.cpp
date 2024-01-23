#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/UIItemManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

LoadingScene::LoadingScene(SceneManager& manager):SceneBase(manager)
{
}

LoadingScene::~LoadingScene()
{
}

void LoadingScene::Init()
{
	//短縮化
	auto& font = FontsManager::GetInstance();

	//インスタンス化
	UIManager_ = std::make_shared<UIItemManager>();

	//UI画像作成
	UIManager_->AddingMenuWithSplitStr(Game::screen_width - 230, Game::screen_height - 80, Game::screen_width, Game::screen_height, "Loading...", font.GetFontHandle("ピグモ 0042"));

	//非同期初期の開始
	SetUseASyncLoadFlag(true);

	//モデルのロード
	ModelManager::GetInstance().LoadModel();

	//外部ファイルのロード
	ExternalFile::GetInstance().LoadFile();

	//データのロード
	ExternalFile::GetInstance().LoadArrangementData();

	//サウンドファイルのロード
	SoundManager::GetInstance().LoadSound();
}

void LoadingScene::End()
{
}

void LoadingScene::Update()
{
	//非同期読み込み中の数を取得
	aSyncLoadNum_ = GetASyncLoadNum();

	if (aSyncLoadNum_ < 1) {
		SetUseASyncLoadFlag(false);
		//manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
	}
}

void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - 230, Game::screen_height - 80);
}
