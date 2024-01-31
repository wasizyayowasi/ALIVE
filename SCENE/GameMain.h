#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class Camera;
class Player;
class CheckCollisionModel;
class ObjectManager;
class ShotManager;
class Tutorial;

class GameMain : public SceneBase
{
public:
	///コンストラクタ
	GameMain(SceneManager& manager);
	//デストラクタ
	virtual ~GameMain();

	//初期化用関数
	void Init();
	//削除用関数
	void End();

	//更新
	void Update();
	//描画
	void Draw();
private:

	int skyHandle_ = -1;

	int makeScreenHandle_ = -1;
	int totalDeathNum_ = 0;					//ゲーム開始からの総死亡数

	bool isFilterOn_ = false;				//フィルターを使用するか

	//消すかもしれない
	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };

	std::shared_ptr<Camera> camera_;			//カメラのシェアードポインタ
	std::shared_ptr<Player> player_;			//プレイヤーのシェアードポインタ
	std::shared_ptr<ObjectManager> objManager_;
	std::shared_ptr<ShotManager> shotManager_;
	std::shared_ptr<Tutorial> tutorial_;
	
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	void (GameMain::* updateFunc_)();		//メンバ関数ポインタ

};

