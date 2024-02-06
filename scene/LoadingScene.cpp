#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"
#include "../util/UIItemManager.h"

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

	//外部ファイルのロード
	ExternalFile::GetInstance().LoadFile();
	
	//モデルのロード
	ModelManager::GetInstance().LoadModel();
	
	//サウンドファイルのロード
	SoundManager::GetInstance().LoadSound();
	
	//画像のロード
	EffectManager::GetInstance().Load();

	//非同期処理を終了する
	SetUseASyncLoadFlag(false);

	loadingFile_ = true;
}

void LoadingScene::End()
{
}

void LoadingScene::Update()
{
	//非同期読み込み中の数を取得
	aSyncLoadNum_ = GetASyncLoadNum();

	if (loadingFile_) {
		if (aSyncLoadNum_ < 1) {
			loadingFile_ = false;
			
			//データのロード
			ExternalFile::GetInstance().LoadArrangementData();
		}
	}
	else {
		if (aSyncLoadNum_ < 1) {
			//manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
		}
	}
}

void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - 230, Game::screen_height - 80);
	//DrawFormatString(0, 0, 0xffffff, "%d", aSyncLoadNum_);
}
