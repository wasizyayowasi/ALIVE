#include "SceneTitle.h"
#include "GameMain.h"
#include "ScenePause.h"
#include "SceneMovie.h"
#include "SceneManager.h"
#include "SelectChapterScene.h"
#include "SettingSceneForSceneTitle.h"

#include "../object/Camera.h"
#include "../object/LightBulb.h"
#include "../object/ObjectManager.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"
#include "../util/GraphManager.h"
#include "../util/UIItemManager.h"

#include <algorithm>

//コンストラクタ
SceneTitle::SceneTitle(SceneManager& manager): SceneBase(manager)
{
	//短縮化
	auto& file  = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//インスタンス化
	UI_					= std::make_shared<UIItemManager>();
	objManager_			= std::make_shared<ObjectManager>();
	subPlayerModel_		= std::make_shared<Model>(model.GetModelHandle(ObjectName[static_cast<int>(ObjectType::Player)]),Material::Other);
	mainPlayerModel_	= std::make_shared<Model>(model.GetModelHandle(ObjectName[static_cast<int>(ObjectType::Player)]),Material::Other);
	camera_				= std::make_shared<Camera>(file.GetCameraTargetPos("start"),file.GetCameraTargetPos("startTargetPos"));
	lightBulb_			= std::make_shared<LightBulb>(model.GetModelHandle(ObjectName[static_cast<int>(ObjectType::LightBulb)]), file.GetSpecifiedInfo("title", "LightBulb"));

	//メインプレイヤーモデルの配置データをセットする
	auto mainPlayerInfo = file.GetSpecifiedInfo("title", "Player");
	mainPlayerModel_->SetScale(mainPlayerInfo.scale);
	mainPlayerModel_->SetPos(mainPlayerInfo.pos);
	mainPlayerModel_->SetRot(mainPlayerInfo.rot);
	mainPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::StopTimer), false, true);

	//サブプレイヤーモデルの配置データをセットする
	auto subPlayerInfo = file.GetSpecifiedInfo("title", "SubPlayer");
	subPlayerModel_->SetScale(subPlayerInfo.scale);
	subPlayerModel_->SetPos(subPlayerInfo.pos);
	subPlayerModel_->SetRot(subPlayerInfo.rot);
	subPlayerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Jump), false, true);

	//オブジェクトの生成
	objManager_->OpeningStageObjectGenerator();

	//ライトの設定
	LightSetting();

	//カメラの配置等の設定
	CameraSettingPos();

	//UIを表示する座標を取得
	menuDrawPos_["SE"]				= file.GetUIPos("SEUIPos");
	menuDrawPos_["BGM"]				= file.GetUIPos("BGMUIPos");
	menuDrawPos_["終了"]			= file.GetUIPos("endDrawPos");
	menuDrawPos_["設定"]			= file.GetUIPos("settingDrawPos");
	menuDrawPos_["戻る"]			= file.GetUIPos("backUIPos");
	menuDrawPos_["モード"]			= file.GetUIPos("windowModeUIPos");
	menuDrawPos_["タイトル"]		= file.GetUIPos("titleDrawPos");
	menuDrawPos_["操作設定"]		= file.GetUIPos("advancedSettingUIPos");
	menuDrawPos_["シーン選択"]		= file.GetUIPos("continueDrawPos");
	menuDrawPos_["ニューゲーム"]	= file.GetUIPos("startDrawPos");

	//UI文字列の作成
	menuName_.push_back("ニューゲーム");
	menuName_.push_back("シーン選択");
	menuName_.push_back("設定");
	menuName_.push_back("終了");

	//UI画像の作成
	fontHandle_ = FontsManager::GetInstance().GetFontHandle("ピグモ 0042");
	float y = 120.0f;
	for (auto& menu : menuName_) {
		UI_->AddMenu(static_cast<float>(Game::screen_width / 2), static_cast<float>(Game::screen_height / 2) + y, 320, 100, menu.c_str(), fontHandle_);
		y += 40.0f;
	}
}

//デストラクタ
SceneTitle::~SceneTitle()
{
	for (auto& light : lightHandle_) {
		DeleteLightHandle(light);
	}
	DeleteLightHandleAll();
}

//初期化
void SceneTitle::Init()
{
	updateFunc_ = &SceneTitle::FadeInUpdate;

	fadeTimer_ = 0;
	fadeValue_ = 0;
}

//終了
void SceneTitle::End()
{
}

//更新
void SceneTitle::Update()
{
	//カメラの更新
	camera_->EasingMoveCamera();

	//電球の更新
	lightBulb_->Update();

	//ライトの更新
	SetLightDirection(lightBulb_->GetFrontVec());
	SetLightPosition(lightBulb_->GetFramePos());

	//ライトの方向設定
	SetLightDirectionHandle(lightHandle_[0], lightDir_[0]);
	SetLightDirectionHandle(lightHandle_[1], lightDir_[1]);

	(this->*updateFunc_)();
}

//描画
void SceneTitle::Draw()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//カメラの初期化
	camera_->Init(VGet(0, 140, 0));

	//オブジェクトの描画
	objManager_->Draw({ 0,0,0 });

	//電球の描画
	lightBulb_->Draw();

	//モデルの描画
	mainPlayerModel_->Draw();
	subPlayerModel_->Draw();

	//UIの描画
	UI_->DrawBillBoard(menuDrawPos_,UIfadeValue_,200.0f);

	if (input.LastInputDevice()) {
		//キー画像描画
		input.DrawKeyGraph(static_cast<int>(InputType::Up)	 , Game::screen_width / 128 * 92 , Game::screen_height - 90, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Left) , Game::screen_width / 128 * 87 , Game::screen_height - 40, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Down) , Game::screen_width / 128 * 92 , Game::screen_height - 40, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Right), Game::screen_width / 128 * 97 , Game::screen_height - 40, 0.8f);
		input.DrawKeyGraph(static_cast<int>(InputType::Space), Game::screen_width / 128 * 112, Game::screen_height - 40, 0.8f);
	}
	else {
		input.DrawPadGraph(static_cast<int>(XboxBotton::Up)     , Game::screen_width / 128 * 92 , Game::screen_height - 90, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::Left)	, Game::screen_width / 128 * 87 , Game::screen_height - 40, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::Down)   , Game::screen_width / 128 * 92 , Game::screen_height - 40, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::Right)  , Game::screen_width / 128 * 97 , Game::screen_height - 40, 0.66f);
		input.DrawPadGraph(static_cast<int>(XboxBotton::A)      , Game::screen_width / 128 * 112, Game::screen_height - 40, 0.66f);
	}
	
	//文字列の描画
	DrawStringToHandle(Game::screen_width / 10 * 8 - 25, Game::screen_height - 60, "移動", 0xffffff, fontHandle_);
	input.DrawName(static_cast<int>(InputType::Space), Game::screen_width / 20 * 18, Game::screen_height - 60, 0xffffff, fontHandle_, true, true, "/");

	//fadeValue_の値によって透過具合が変化するタイトルの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UIfadeValue_);
	DrawBillboard3D(menuDrawPos_["タイトル"], 0.5f, 0.5f, 300.0f, 0.0f, GraphManager::GetInstance().GetGraph("title"), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//黒いフェード用boxの描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//ライトの設定
void SceneTitle::LightSetting()
{
	SetUseLighting(true);

	//スポットライトに変更する。ライトの設定
	outAngle_ = 90.0f * DX_PI_F / 180.0f;
	inAngle_ = 30.0f * DX_PI_F / 180.0f;
	ChangeLightTypeSpot(lightBulb_->GetFramePos(), lightBulb_->GetFrontVec(), outAngle_, inAngle_, 600.0f, 0.0f, 0.003f, 0.0f);

	//ライトのディフューズカラーの設定
	SetLightDifColor(GetColorF(0.87f, 0.72f, 0.52f, 1.0f));

	//ディレクションライト角度取得
	VECTOR dir = {};
	dir.x = -33 / DX_PI_F * 180.0f;
	dir.y = -33 / DX_PI_F * 180.0f;
	dir.z = -33 / DX_PI_F * 180.0f;

	//ディレクションライトの作成
	lightHandle_.push_back(CreateDirLightHandle(dir));
	lightDir_.push_back(dir);
	SetLightDifColorHandle(lightHandle_[0], GetColorF(0.41f, 0.41f, 0.41f, 1.0f));

	//ディレクションライト角度取得
	dir.x = 33 / DX_PI_F * 180.0f;
	dir.y = -33 / DX_PI_F * 180.0f;
	dir.z = 33 / DX_PI_F * 180.0f;

	//ディレクションライトの作成
	lightHandle_.push_back(CreateDirLightHandle(dir));
	lightDir_.push_back(dir);
	SetLightDifColorHandle(lightHandle_[1], GetColorF(0.41f, 0.41f, 0.41f, 1.0f));
}

//カメラの設定
void SceneTitle::CameraSettingPos()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	//カメラの初期化
	camera_->Init(VGet(0, 140, 0));

	//カメラのポジションと見る位置の設定
	camera_->SetCameraTargetPosAndView(file.GetCameraTargetPos("start"), file.GetCameraTargetPos("startTargetPos"), { 0,1,0 });

	//カメラ情報の設定
	CameraInfo info = {};

	//設定オブジェクトを見るカメラ
	info.targetPos = file.GetCameraTargetPos("setting");
	info.targetView = file.GetCameraTargetPos("settingTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//ベッドを見るカメラ
	info.targetPos = file.GetCameraTargetPos("start");
	info.targetView = file.GetCameraTargetPos("startTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);

	//本を見るカメラ
	info.targetPos = file.GetCameraTargetPos("continue");
	info.targetView = file.GetCameraTargetPos("continueTargetPos");
	info.upVec = VGet(0, 0, 1);

	cameraInfo_.push_back(info);

	//扉を見るカメラ
	info.targetPos = file.GetCameraTargetPos("end");
	info.targetView = file.GetCameraTargetPos("endTargetPos");
	info.upVec = VGet(0, 1, 0);

	cameraInfo_.push_back(info);
}

//選択番号の更新
void SceneTitle::SelectNumUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	bool isTriggerLeft = input.IsTriggered(InputType::Left);
	bool isTriggerRight = input.IsTriggered(InputType::Right);
	bool isTriggerUp = input.IsTriggered(InputType::Up);
	bool isTriggerDown = input.IsTriggered(InputType::Down);

	switch (selectNum_)
	{
	case 0:
		if (isTriggerRight || isTriggerDown)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 1;
		}
		break;
	case 1:
		if (isTriggerUp)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 2;
		}
		else if (isTriggerLeft)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 0;
		}
		else if (isTriggerRight)
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 3;
		}
		break;
	case 2:
		if (isTriggerLeft || isTriggerDown) 
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 1;
		}
		break;
	case 3:
		if (isTriggerLeft || isTriggerDown) 
		{
			oldSelectNum_ = selectNum_;
			selectNum_ = 1;
		}
		break;
	}
}

//フェードインの更新
void SceneTitle::FadeInUpdate()
{
	//フェードイン
	float timer = (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_));
	UIfadeValue_ = static_cast <int>(255 * timer);
	if (++fadeTimer_ == fadeInterval_) {
		updateFunc_ = &SceneTitle::UIUpdate;
		UIfadeValue_ = 255;
		return;
	}
}

//UIの更新
void SceneTitle::UIUpdate()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();
	auto& input = InputState::GetInstance();

	if (camera_->GetMoving()) {
		return;
	}

	//選択
	SelectNumUpdate();

	//現在のフレームと前のフレームで番号が違ったら
	//カメラの設定を行う
	if (selectNum_ != oldSelectNum_) {
		camera_->SetCameraTargetPosAndView(cameraInfo_[selectNum_].targetPos, cameraInfo_[selectNum_].targetView, cameraInfo_[selectNum_].upVec);
		camera_->LesetElapsedTime();
	}

	//決定
	if (input.IsTriggered(InputType::Space)) {
		updateFunc_ = &SceneTitle::UIFadeOutUpdate;
	}

	//動画を再生する
	if (input.IsTriggered(InputType::Pause)) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneMovie>(manager_)));
	}
}

//オープニングの更新
void SceneTitle::OpeningUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	//アニメーション番号チェック
	bool checkStopTimerAnim = PlayerAnimType::StopTimer == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkStopTimerCancelAnim = PlayerAnimType::StopTimerCancel == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());
	bool checkWakeUpAnim = PlayerAnimType::WakeUp == static_cast<PlayerAnimType>(mainPlayerModel_->GetCurrentAnimNo());

	//モデルの更新
	mainPlayerModel_->Update();

	//時計をとめるアニメーションの後仰向けに戻る
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerAnim) {
		sound.StopSE("alarm");
		sound.PlaySE("stopAlarm");
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::StopTimerCancel), false, false, 10);
	}

	//仰向けになるアニメーションが終わった後
	if (mainPlayerModel_->IsAnimEnd() && checkStopTimerCancelAnim) {
		mainPlayerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::WakeUp), false, false, 10);
	}

	//起き上がるアニメーションが終わったら
	if (mainPlayerModel_->IsAnimEnd() && checkWakeUpAnim) {
		updateFunc_ = &SceneTitle::SceneTitleFadeOutUpdate;
	}
}

//UIのフェードアウト
void SceneTitle::UIFadeOutUpdate()
{
	//UIのフェードアウト
	UIfadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		UIfadeValue_ = 0;
		SceneChange();
		return;
	}
}

//タイトルのフェードアウト
void SceneTitle::SceneTitleFadeOutUpdate()
{
	//フェードアウト
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<GameMain>(manager_)));
		return;
	}
}

//シーンを切り替える
void SceneTitle::SceneChange()
{
	switch (selectNum_) {
	case 0:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SettingSceneForSceneTitle>(manager_)));
		break;
	case 1:
		updateFunc_ = &SceneTitle::OpeningUpdate;
		SoundManager::GetInstance().PlaySE("alarm");
		break;
	case 2:
		Init();
		manager_.PushFrontScene(std::shared_ptr<SceneBase>(std::make_shared<SelectChapterScene>(manager_)));
		break;
	case 3:
		manager_.SetEndFlag(true);
		break;
	}
}
