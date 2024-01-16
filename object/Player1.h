#pragma once
#include "PlayerData.h"
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>

class InputState;
class Model;
class ObjectManager;
class ObjectBase;
class ManualCrank;
class Lever;

template <class TemplateState> class StatePattern;

class Player1
{
public:

	enum class State {
		Normal,
		Jump,
		Death,
		Sit,
		Crank,
		Lever,
	};

	//update関数
	//通常更新
	void NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//移動処理
	void Move(const InputState& input);

	/// <summary>
	/// 走りジャンプではないときのジャンプ
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void JumpUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	/// <summary>
	/// プレイヤーの死体に与える情報を作る関数
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void DeathUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//座っている途中
	void IdleToSitup(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// プレイヤーに座るアニメーションをさせる関数
	/// </summary>
	/// <param name="input">外部装置の入力情報を参照する</param>
	void SitUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);


	//クランクを回す
	void GoCrankRotationPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	void CrankUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// 投擲物との衝突アップデート
	/// </summary>
	void BulletHitMeUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

	//レバーを倒す
	void GoLeverPullPosition(const InputState& input, std::shared_ptr<ObjectManager> objManager);
	void LeverUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager);

private:
	//待機処理
	void ChangeAnimIdle();
	//移動ベクトルと回転の制御
	float ControlOfTranslationVecAndRot(const InputState& input);
	//回転処理
	void RotationUpdate();
	//死体の後処理
	void DeathPersonPostProsessing(std::shared_ptr<ObjectManager> objManager);
	/// <summary>
	/// プレイヤーの死体をvector配列で管理する関数
	/// </summary>
	void DeadPersonGenerater(std::shared_ptr<ObjectManager> objManager);
	//持ち運ぶ
	void CarryObjectUpdate();
	//荷物を下ろす
	void DropOffObjectUpdate();
	/// <summary>
	/// クランクを回転させるアップデート
	/// </summary>
	void CrankRotationUpdate(float rotZ);
};

