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

#include <DxLib.h>

GameEnd::GameEnd(SceneManager& manager) : SceneBase(manager),updateFunc_(&GameEnd::fadeInUpdate)
{
	//短縮化
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//インスタンス化
	objManager_ = std::make_shared<ObjectManager>();
	camera_ = std::make_shared<Camera>(VGet(265, 314, -803), VGet(265, 314, 1777));
	playerModel_ = std::make_shared<Model>(model.GetModelHandle(ObjectType::Player), Material::Other);

	//モデルの設定
	auto info = file.GetSpecifiedInfo("end", "player");
	playerModel_->SetPos(info.pos);
	playerModel_->SetRot(info.rot);
	playerModel_->SetScale(info.scale);
	//アニメーションの変更
	playerModel_->SetAnimation(static_cast<int>(PlayerAnimType::Run), true, true);

	//ライト処理を行わない
	SetUseLighting(false);

	//オブジェクトの生成
	objManager_->EndStageObjectGenerator();

	//プレイヤーの手の座標を取得
	VECTOR pos = playerModel_->GetFrameLocalPosition("hand.R_end");
	float height = 0.0f;

	//死体モデルの生成
	for (int i = 0; i < file.GetDeathCount(); i++) {
		corpseModel_.push_back(std::make_pair(false, std::make_shared<Model>(model.GetModelHandle(ObjectType::Player), Material::Other)));
		corpseModel_[i].second->SetPos({ pos.x,pos.y + height,pos.z });
		corpseModel_[i].second->SetRot(info.rot);
		corpseModel_[i].second->SetScale(info.scale);
		corpseModel_[i].second->SetAnimEndFrame(static_cast<int>(PlayerAnimType::Death));
		//マテリアルの色を変える
		MV1SetMaterialDifColor(corpseModel_[i].second->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
		height += 15.0f;
	}

	//死体を置く場所を画面を分割して決める
	//画面の分割数の取得
	divisionNum_ = file.GetDeathCount() / 5 + 1;
	if (file.GetDeathCount() % 5 > 0) {
		divisionNum_++;
	}
}

GameEnd::~GameEnd()
{
}

void GameEnd::Init()
{
}

void GameEnd::End()
{
}

void GameEnd::Update()
{
	(this->*updateFunc_)();
}

void GameEnd::Draw()
{
	camera_->Init(VGet(265, 314, 1777));

	//プレイヤーモデルの描画
	playerModel_->Draw();

	//死体モデルの描画
	for (auto& corpse : corpseModel_) {
		corpse.second->Draw();
	}

	//オブジェクトの描画
	objManager_->Draw({ 0,0,0 });

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, fadeValue_);
	//画面全体を真っ黒に塗りつぶす
	DrawBox(0, 0, Game::screen_width, Game::screen_height, fadeColor_, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameEnd::fadeInUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (--fadeTimer_ == 0) {
		updateFunc_ = &GameEnd::normalUpdate;
		fadeValue_ = 0;
	}
}

void GameEnd::normalUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//操作する死体の番号
	static int corpseNum = 0;

	//プレイヤーモデルの座標
	VECTOR playerPos = playerModel_->GetPos();

	//プレイヤーモデルの更新
	playerModel_->Update();

	//プレイヤーモデルの座標変更
	playerModel_->SetPos(VAdd(playerPos, { 1.0f,0,0 }));

	//操作する死体の番号が死体の総数を超えたらリターン
	if (corpseNum > corpseModel_.size()) {
		return;
	}

	//プレイヤーの座標をスクリーン座標にする
	VECTOR screenPos = ConvWorldPosToScreenPos(playerModel_->GetPos());

	//死体の高さの補正
	float height = 0.0f;

	//画面を分割した後の一つ当たりの横の長さ
	int divisionPosX = Game::screen_width / divisionNum_;

	//プレイヤーの座標を元に取得したスクリーン座標を
	//上記のサイズで余りを求める
	int errorRange = static_cast<int>(screenPos.x) % divisionPosX;

	//誤差範囲内だったら、死体をプレイヤーの座標に置く
	if (errorRange >= -1 && errorRange <= 1) {
		for (int i = corpseNum; i < corpseNum + 5; i++) {
			//現在の番号が死体の総数を超えたら以降の処理を行わない
			if (i > corpseModel_.size() - 1) {
				continue;
			}

			//死体の座標をプレイヤーの座標に設定する
			corpseModel_[i].second->SetPos({ playerPos.x,playerPos.y + height, playerPos.z });
			//フラグをtrueにする
			corpseModel_[i].first = true;

			//高さを変更する
			height += 15.0f;
		}

		//死体を5個足元に置いた後、まだ手元に死体が残っていたら
		//操作する死体の番号範囲を変える
		if ((corpseModel_.size() - 1 - corpseNum + 1) % 5 > 0) {
			corpseNum += 5;
		}
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

	if (input.IsTriggered(InputType::space)) {
		updateFunc_ = &GameEnd::fadeOutUpdate;
	}
}

void GameEnd::fadeOutUpdate()
{
	fadeValue_ = static_cast <int>(255 * (static_cast<float>(fadeTimer_) / static_cast<float>(fadeInterval_)));
	if (++fadeTimer_ == fadeInterval_) {
		manager_.ChangeScene(std::shared_ptr<SceneBase>(std::make_shared<SceneTitle>(manager_)));
		return;
	}
}
