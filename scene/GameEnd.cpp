#include "GameEnd.h"
#include "SceneTitle.h"
#include "SceneManager.h"

#include "../object/Camera.h"
#include "../object/objectManager.h"

#include "../util/game.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ModelManager.h"
#include "../util/ExternalFile.h"
#include "../util/FontsManager.h"
#include "../util/SoundManager.h"

#include <algorithm>

namespace {
	//棒グラフの横のサイズ
	constexpr int bar_graph_width = 20;

	//棒グラフの縦のサイズ
	constexpr int bar_graph_height = 10;

	//横の分割数
	constexpr int division_width = 10;

	//縦の分割数
	constexpr int division_height = 15;
}

//コンストラクタ
GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::FadeInUpdate)
{
	//短縮化
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& font = FontsManager::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//インスタンス化
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(265, 314, -803), VGet(265, 314, 1777));
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::Player)].name), Material::Other);
	boardModel_  = std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::WhiteBoard)].name), Material::Iron);

	//モデルの設定
	auto info = file.GetSpecifiedInfo("end", "player");
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetScale(info.scale);
	//アニメーションの変更
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//モデルの設定
	info = file.GetSpecifiedInfo("end", "WhiteBoard");
	boardModel_->SetPos(info.pos);
	boardModel_->SetRot(info.rot);
	boardModel_->SetScale(info.scale);

	//オブジェクトの生成
	objManager_->EndStageObjectGenerator();

	//プレイヤーの手の座標を取得
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	float height = 0.0f;

	info = file.GetSpecifiedInfo("end", "player");

	//死体モデルの生成
	for (int i = 0; i < file.GetTotalDeathNum().back(); i++) {
		corpseModel_.push_back(std::make_pair(false, std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::Player)].name), Material::Other)));
		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		corpseModel_[i].second->SetRot(info.rot);
		corpseModel_[i].second->SetScale(info.scale);
		corpseModel_[i].second->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));
		//マテリアルの色を変える
		MV1SetMaterialDifColor(corpseModel_[i].second->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		height += 15.0f;
	}

	//棒グラフの初期高さ
	for (int i = 0; i < 5;i++) {
		barGraphHeight_.push_back(Game::screen_height / division_height * 12);
	}

	//死体を置く場所を画面を分割して決める
	//画面の分割数の取得
	divisionNum_ = file.GetTotalDeathNum().back() / 5 + 1;
	if (file.GetTotalDeathNum().back() % 5 > 0) {
		divisionNum_++;
	}

	//フォントハンドルの取得
	pigumo21FontHandle_ = font.GetFontHandle("ピグモ 0021");
	pigumo42FontHandle_ = font.GetFontHandle("ピグモ 0042");

	//ライト処理を行わない
	SetUseLighting(false);

	sound.Set3DSoundListenerInfo(camera_->GetPos(), camera_->GetTarget());
}

//デストラクタ
GameEnd::~GameEnd()
{
}

//初期化
void GameEnd::Init()
{
}

//終了
void GameEnd::End()
{
}

//更新
void GameEnd::Update()
{
	(this->*updateFunc_)();
}

//描画
void GameEnd::Draw()
{
	//短縮化
	auto& file = ExternalFile::GetInstance();

	camera_->Init(VGet(265, 314, 1777));

	//プレイヤーモデルの描画
	playerModel_->Draw();

	//ホワイトボードの描画
	boardModel_->Draw();

	//死体モデルの描画
	for (auto& corpse : corpseModel_) {
		corpse.second->Draw();
	}

	//オブジェクトの描画
	objManager_->Draw({ 0,0,0 });

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int型のスクリーン座標
	int castScreenPosX = static_cast<int>(screenPos.x);

	//色
	int color = 0x0000ff;

	for (int i = 0; i < file.GetTotalDeathNum().size();i++) {

		barGraphHeight_[i] = (std::max)(barGraphHeight_[i] - 1, Game::screen_height / division_height * 12 - file.GetTotalDeathNum()[i] * bar_graph_height);

		if (static_cast<int>(file.GetTotalDeathNum().size() - 1) - i == 0) {
			color = 0xff0000;
		}

		//棒グラフの描画
		DrawBox(Game::screen_width / division_width * (i + 3) - bar_graph_width - (Game::screen_width / 2 - castScreenPosX),
				barGraphHeight_[i],
				Game::screen_width / division_width * (i + 3) + bar_graph_width - (Game::screen_width / 2 - castScreenPosX),
				Game::screen_height / division_height * 12,
				color, true);

		//フォントを適用した文字列のサイズを取得
		int size = GetDrawFormatStringWidthToHandle(pigumo42FontHandle_,"%d体", file.GetTotalDeathNum()[i]);

		//数字の描画
		DrawFormatStringToHandle(Game::screen_width / division_width * (i + 3) - size / 2 - (Game::screen_width / 2 - castScreenPosX),
								 Game::screen_height / division_height * 12 - file.GetTotalDeathNum()[i] * bar_graph_height - size,
								 0x000000, pigumo42FontHandle_, "%d体", file.GetTotalDeathNum()[i]);

		if (4 - i == 0) {
			DrawVStringToHandle(Game::screen_width / division_width * (i + 3) - 10 - (Game::screen_width / 2 - castScreenPosX),
								Game::screen_height / division_height * 12 + 15,
								"今回", 0xff0000, pigumo21FontHandle_);
			continue;
		}

		DrawFormatVStringToHandle(Game::screen_width / division_width * (i + 3)-10 - (Game::screen_width / 2 - castScreenPosX),
								  Game::screen_height / division_height * 12+15,
								  0x000000,
								  pigumo21FontHandle_,
								  "%d回前", 4 - i);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//死体の更新
void GameEnd::CorpseUpdate(const VECTOR playerPos)
{
	//死体の高さの補正
	float height = 0.0f;

	//死体のモデルを回転させる
	if (isTurn_) {
		for (int i = 0; i < corpseModel_.size(); i++) {
			if (corpseModel_[i].first) {
				continue;
			}
			VECTOR rot = {};
			rot.y = 0 * DX_PI_F / 180.0f;
			corpseModel_[i].second->SetRot(rot);
		}
	}

	//アニメーションが終了したら、足元に死体を置く
	if (playerModel_->IsAnimEnd()) {
		for (int i = corpseNum_; i < corpseNum_ + 5; i++) {
			//現在の番号が死体の総数を超えたら以降の処理を行わない
			if (i > static_cast<int>(corpseModel_.size() - 1)) {
				continue;
			}

			//死体の座標をプレイヤーの座標に設定する
			corpseModel_[i].second->SetPos({ playerPos.x,playerPos.y + height, playerPos.z });

			//フラグをtrueにする
			corpseModel_[i].first = true;

			//高さを変更する
			height += 15.0f;
		}

		//まだ置かれていない死体のモデルを回転させる
		for (int i = 0; i < corpseModel_.size(); i++) {
			if (corpseModel_[i].first) {
				continue;
			}

			VECTOR rot = {};
			rot.y = -90 * DX_PI_F / 180.0f;
			corpseModel_[i].second->SetRot(rot);
		}

		//死体を5個足元に置いた後、まだ手元に死体が残っていたら
		//操作する死体の番号範囲を変える
		if ((static_cast<int>(corpseModel_.size()) - 1 - corpseNum_ + 1) / 5 > 0) {
			corpseNum_ += 5;
		}

		//フラグを折る
		isPutting_ = false;
	}

	//プレイヤーモデルの手の座標を取得
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	height = 0.0f;

	//死体モデルの座標を変更
	for (int i = 0; i < corpseModel_.size(); i++) {
		if (corpseModel_[i].first) {
			continue;
		}

		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		height += 15.0f;
	}
}

//ホワイトボードの更新
void GameEnd::WhiteBoardUpdate()
{
	//短縮化
	auto& sound = SoundManager::GetInstance();

	if (isWentToRightSide_) {
		return;
	}

	if (!isResultDisplaying_) {
		boardModel_->SetPos(VAdd(boardModel_->GetPos(), moveVec_));
		boardIsMoving_ = true;
		if (!sound.CheckSoundFile("pull")) {
			sound.PlaySE("pull");
		}
	}

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(boardModel_->GetPos());

	//int型のスクリーン座標
	int castScreenPosX = static_cast<int>(screenPos.x);

	//画面の半分のサイズ(横)
	int screenWidthHalf = Game::screen_width / 2;

	if (castScreenPosX >= screenWidthHalf - 1 && castScreenPosX <= screenWidthHalf + 1) {
		boardIsMoving_ = false;
		isResultDisplaying_ = true;
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), true, false, 10);
	}

	//ホワイトボードが画面外に出たらフェードアウトする
	if (castScreenPosX < -Game::screen_width / 2) {
		updateFunc_ = &GameEnd::FadeOutUpdate;
	}
}

//フェードインの更新
void GameEnd::FadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::NormalUpdate;
		fadeValue_ = 0;
	}
}

//通常時の更新
void GameEnd::NormalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& file = ExternalFile::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//プレイヤーモデルの座標
	VECTOR playerPos = playerModel_->GetPos();

	//プレイヤーモデルの更新
	playerModel_->Update();

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(playerModel_->GetPos());

	//プレイヤーが画面外に行ったら動いているフラグをfalseにする
	if (screenPos.x > Game::screen_width && isWentToRightSide_) {
		isWentToRightSide_ = false;
		moveVec_.x = -1;
		playerModel_->SetPos(file.GetSpecifiedInfo("end", "secondPos").pos);
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Pull), true, false, 10);
		playerPos = playerModel_->GetPos();
	}

	if (!isPutting_ && !isResultDisplaying_) {
		//プレイヤーモデルの座標変更
		playerModel_->SetPos(VAdd(playerPos, moveVec_));
		if (playerModel_->GetSpecifiedAnimTime(6) || playerModel_->GetSpecifiedAnimTime(45)) {
			sound.Set3DSoundInfo(playerPos, 800.0f, "asphaltStep");
			sound.PlaySE("asphaltStep");
		}
	}

	//ホワイトボードの更新
	WhiteBoardUpdate();

	if (input.IsTriggered(InputType::Space) && isResultDisplaying_) {
		isResultDisplaying_ = false;
	}

	if (!isWentToRightSide_) {
		return;
	}

	//画面を分割した後の一つ当たりの横の長さ
	int divisionPosX = Game::screen_width / divisionNum_;

	//プレイヤーの座標を元に取得したスクリーン座標を
	//上記のサイズで余りを求める
	int errorRange = static_cast<int>(screenPos.x) % (divisionPosX * currentDivisionNum_);

	if (screenPos.x > 50 && screenPos.x < Game::screen_width - 50) {
		//誤差範囲内だったら、プレイヤーのアニメーションを変更する
		if (errorRange >= -1 && errorRange <= 1 && !isPutting_) {
			//アニメーションを変更する
			playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Put), false, true, 10);

			//フラグを立てる
			isPutting_ = true;

			currentDivisionNum_++;
		}
	}

	//プレイヤーが特定のアニメーションフレームになったらフラグを立てる
	if (isPutting_) {
		if (playerModel_->GetSpecifiedAnimTime(30)) {
			isTurn_ = true;
		}
		else if (playerModel_->GetSpecifiedAnimTime(40)) {
			isTurn_ = false;
		}
	}

	//死体の更新
	CorpseUpdate(playerPos);

	if (!isPutting_) {
		//アニメーションを変更する
		playerModel_->ChangeAnimation(static_cast<int>(PlayerAnimType::Run), true, false, 10);
	}
}

//フェードアウトの更新
void GameEnd::FadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
