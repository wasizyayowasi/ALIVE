#include "LoadingScene.h"
#include "DebugScene.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../util/game.h"
#include "../util/ModelManager.h"
#include "../util/FontsManager.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/GraphManager.h"
#include "../util/EffectManager.h"
#include "../util/UIItemManager.h"

//コンストラクタ
LoadingScene::LoadingScene(SceneManager& manager):SceneBase(manager)
{
}

//デストラクタ
LoadingScene::~LoadingScene()
{
}

//初期化
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
	GraphManager::GetInstance().Load();

	//エッフェクト画像のロード
	EffectManager::GetInstance().Load();

	//非同期処理を終了する
	SetUseASyncLoadFlag(false);

	loadingFile_ = true;
}

//終了
void LoadingScene::End()
{
}

//更新
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
#ifdef _DEBUG
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<DebugScene>(manager_)));
#else
			manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
#endif
		}
	}
}

//描画
void LoadingScene::Draw()
{
	UIManager_->ChangePosDraw(Game::screen_width - 230, Game::screen_height - 80);
}
