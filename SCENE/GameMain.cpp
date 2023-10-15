#include "GameMain.h"
#include "SceneManager.h"
#include "GameEnd.h"

#include "../gimmick/GimmickBase.h"
#include "../gimmick/Switch.h"
#include "../gimmick/Steelyard.h"

#include "../object/Player.h"
#include "../object/cube.h"
#include "../object/Camera.h"
#include "../object/CharacterBase.h"

#include "../staging/Broom.h"
#include "../staging/DepthOfField.h"

#include "ScenePause.h" 

#include "../util/game.h"
#include "../util/SoundManager.h"
#include "../util/InputState.h"
#include "../util/model.h"
#include "../util/LoadExternalFile.h"

#include "../util/ObjectManager.h"
#include "../util/ObjectType.h"

using namespace std;

namespace {
	const char* const player_Filename = "data/player/player14.mv1";

	const char* const temp_fieldpath = "data/model/tempFiled4.mv1";
	const char* const temp_stairs = "data/model/stairs.mv1";
	const char* const box_filename = "data/model/box.mv1";
	const char* const bigPillar_filename = "data/level0_model/bigPillar.mv1";
	const char* const switch_filename = "data/model/switch.mv1";
	const char* const steelyard_filename = "data/model/steelyard.mv1";
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

GameMain::GameMain(SceneManager& manager, bool continuation) : SceneBase(manager),
updateFunc_(&GameMain::fadeInUpdate),
isContinuation_(continuation)
{
}

GameMain::~GameMain()
{
	//ゲームメインが終わるときにプレイ中に死んだ回数と
	//saveDataに記録されている死亡回数を足す
	totalDeathNum_ += player_->getDeathCount();
	LoadExternalFile::GetInstance(isContinuation_).SaveDataRewriteInfo(checkPoint_, totalDeathNum_);
}

void GameMain::Init()
{
	makeScreenHandle_ = MakeScreen(Game::screen_width, Game::screen_height, true);

	//オブジェクトを生成
	ObjectGenerater();

	//カメラのインスタンス化
	camera_ = make_shared<Camera>();
	//プレイヤーのインスタンス化
	player_ = make_shared<Player>(player_Filename);
	//broom_ = make_shared<Broom>();
	//depthOfField_ = make_shared<DepthOfField>();

	//仮でライト処理を消している
	SetUseLighting(false);

	//1mの範囲を設定する
	Set3DSoundOneMetre(10.0f);

	//セーブデータの内容を読み取る
	auto data = LoadExternalFile::GetInstance(isContinuation_);
	//死亡回数
	totalDeathNum_ = data.GetSaveData().totalDeathNum;
	//
	player_->SetSaveData(data.GetSaveData().checkPoint,isContinuation_);

	//プレイヤーの初期化
	player_->Init();

	//3Dリスナーの位置を設定する
	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
	//3Dサウンドに関連する情報を設定する
	SoundManager::GetInstance().Set3DSoundInfo(VGet(575, 120, -60), 1000, "cafe");
	//仮でcafeという音楽を流している
	SoundManager::GetInstance().Play("cafe");
}

void GameMain::End()
{
	DeleteGraph(makeScreenHandle_);
}

//更新
void GameMain::Update(const InputState& input)
{
	(this->*updateFunc_)(input);
}

//描画
void GameMain::Draw()
{
	SetDrawScreen(makeScreenHandle_);
	ClearDrawScreen();

	camera_->init();
	camera_->TrackingCameraUpdate(player_->GetStatus().pos);

	//broom_->writingScreenUpdate(player_->getPos());
	DrawString(0, 0, "GameMain", 0xffffff);

	player_->Draw();
	
	ObjectManager::GetInstance().Draw();

	//broom_->graphFilterUpdate();
	//broom_->draw();

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, makeScreenHandle_, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//オブジェクトの生成
void GameMain::ObjectGenerater()
{
	auto& objManager = ObjectManager::GetInstance();

	objManager.ObjectGenerator(ObjectBaseType::enemyBase, ObjectType::enemy);
	objManager.ObjectGenerator(ObjectBaseType::ornamentBase, ObjectType::field);
	objManager.ObjectGenerator(ObjectBaseType::carryBase, ObjectType::carry);
	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch);
	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSwitch);
	objManager.ObjectGenerator(ObjectBaseType::gimmickBase, ObjectType::gimmickSteelyard);
	objManager.ObjectGenerator(ObjectBaseType::characterBase, ObjectType::deadPerson);
}

//TODO：別のフェードインが出来次第消去
void GameMain::fadeInUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameMain::normalUpdate;
		fadeValue_ = 0;
	}
}

//更新
void GameMain::normalUpdate(const InputState& input)
{
	auto& objManager = ObjectManager::GetInstance();

	objManager.Update(*player_);

	player_->Update(input);
	camera_->ChangeOfFocus(input);

	//camera_->fixedPointCamera(player_->getPos());

	SoundManager::GetInstance().Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());

	if (player_->GetStatus().pos.x > 1900) {
		checkPoint_ = { 1900,0,0 };
	}

	if (input.IsTriggered(InputType::pause)) {
		//GraphFilter(makeScreenHandle_, DX_GRAPH_FILTER_GAUSS, 32, 20000);
		manager_.PushScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
	}
}

//TODO：別のフェードインが出来次第消去
void GameMain::fadeOutUpdate(const InputState& input)
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<ScenePause>(manager_)));
		return;
	}
}
