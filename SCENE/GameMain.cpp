#include "GameMain.h"
#include "GameEnd.h"
#include "ScenePause.h" 
#include "SceneManager.h"

#include "../gimmick/Switch.h"
#include "../gimmick/GimmickBase.h"

#include "../object/Player.h"
#include "../object/Camera.h"
#include "../object/ShotManager.h"
#include "../object/CharacterBase.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "../util/game.h"
#include "../util/model.h"
#include "../util/Tutorial.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/EffectManager.h"
#include "../util/CheckCollisionModel.h"

#include "../object/ObjectManager.h"
#include "../object/ObjectData.h"

GameMain::GameMain(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameMain::FadeInUpdate)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//インスタンス化
	player_ = std::make_shared<Player>(file.GetSpecifiedInfo("main", "Player"));
	tutorial_ = std::make_shared<Tutorial>();
	shotManager_ = std::make_shared<ShotManager>();
	objManager_ = std::make_shared<ObjectManager>();
	checkCollisionModel_ = std::make_shared<CheckCollisionModel>();

	VECTOR pos = VAdd(player_->GetStatus().pos, VGet(0, 300, -550));
	camera_ = std::make_shared<Camera>(pos, player_->GetStatus().pos);

	//ゲームオブジェクトの生成
	objManager_->MainStageObjectGenerator();
}

GameMain::~GameMain()
{
	//ゲームメインが終わるときにプレイ中に死んだ回数と
	//saveDataに記録されている死亡回数を足す
	ExternalFile::GetInstance().SaveDataRewriteInfo(totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//仮でライト処理を消している
	SetUseLighting(false);

	//1mの範囲を設定する
	Set3DSoundOneMetre(10.0f);

	skyHandle_ = MV1LoadModel("data/model/skyDorm/SkyDorm.mv1");
	float scale = 30.0f;
	MV1SetScale(skyHandle_, VGet(scale, scale, scale));
	MV1SetPosition(skyHandle_, VGet(0, 200, 0));
}

void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
	MV1DeleteModel(skyHandle_);
}

//更新
void GameMain::Update()
{
	(this->*updateFunc_)();
}

//描画
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	//カメラの初期化
	//SetDrawScreenを行うとカメラの情報がリセットされるために
	camera_->Init(camera_->GetTarget());
	camera_->Update(player_->GetStatus().pos, player_->GetStatus().height);

	//プレイヤーの描画
	player_->Draw();

	//オブジェクトの描画
	objManager_->Draw(player_->GetStatus().pos);

	//エフェクトの描画
	EffectManager::GetInstance().Draw();

	//スカイドームの描画
	MV1SetPosition(skyHandle_, player_->GetStatus().pos);
	MV1DrawModel(skyHandle_);

#ifdef _DEBUG
	camera_->DebugDraw();
#endif // _DEBUG

	//弾の描画
	shotManager_->Draw();

	//チュートリアルの描画
	tutorial_->Draw();

#ifdef _DEBUG
	VECTOR pos = player_->GetStatus().pos;
	DrawFormatString(0, 48, 0xffffff, "%.2f,%.2f,%.2f", pos.x, pos.y, pos.z);
#endif // _DEBUG

	SetDrawScreen(DX_SCREEN_BACK);

	//フィルター処理を行うか
	if (isFilterOn_) {
		GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 800);
	}
	//makescreenHandleに書き込んだ内容を描画する
	DrawGraph(0, 0, makeScreenHandle_, true);

	//画面全体を真っ黒に塗りつぶす
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::NormalUpdate;
		fadeValue_ = 0;
	}
}

//更新
void GameMain::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//フィルター処理を行わない用にする
	isFilterOn_ = false;

	//プレイヤーの更新
	player_->Update(objManager_);

	//オブジェクトの更新
	objManager_->Update(*player_,shotManager_);

	//エフェクトの更新
	EffectManager::GetInstance().Update();

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->CheckCollision(player_,objManager_);

	//弾の更新
	shotManager_->Update();

	//弾とプレイヤーの衝突判定
	shotManager_->Hit(*player_);

	//チュートリアル
	tutorial_->Update(player_->GetStatus().pos);

	//リスナーの位置と方向を設定
	//今回は、プレイヤーではなくカメラの座標にしている
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	//ポーズシーンを開く
	if (input.IsTriggered(InputType::Pause)) {
		isFilterOn_ = true;
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}

	auto info = ExternalFile::GetInstance().GetSpecifiedInfo("main","GOAL");

	auto result = MV1CollCheck_Capsule(	player_->GetModelPointer()->GetModelHandle(),
										player_->GetModelPointer()->GetColFrameIndex(),
										VGet(info.pos.x, info.pos.y - info.scale.y, info.pos.z), 
										VGet(info.pos.x, info.pos.y + info.scale.y, info.pos.z), 600);

	if (result.HitNum > 0) {
		totalDeathNum_ += player_->GetDeathCount();
		ExternalFile::GetInstance().SetDeathCount(totalDeathNum_);
		updateFunc_ = &GameMain::FadeOutUpdate;
	}

	MV1CollResultPolyDimTerminate(result);
}

//TODO：別のフェードインが出来次第消去
void GameMain::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameEnd>(manager_)));
		return;
	}
}
