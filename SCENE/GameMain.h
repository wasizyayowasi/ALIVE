#pragma once
#include "SceneBase.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class Camera;
class Player;
class Broom;
class DepthOfField;
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
	void Update(const InputState& input);
	//描画
	void Draw();

	//オブジェクトの生成
	void ObjectGenerater();

private:

	int debugDrawTime_ = 0;
	int debugUpdateTime_ = 0;

	int skyHandle_ = -1;

	int makeScreenHandle_ = -1;
	int totalDeathNum_ = 0;					//ゲーム開始からの総死亡数

	bool isFilterOn_ = false;				//フィルターを使用するか

	//消すかもしれない
	void FadeInUpdate(const InputState& input);
	void NormalUpdate(const InputState& input);
	void FadeOutUpdate(const InputState& input);

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = 0;
	int fadeValue_ = 0;
	int fadeColor_ = 0x000000;

	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };

	std::shared_ptr<Camera> camera_;			//カメラのシェアードポインタ
	std::shared_ptr<Player> player_;			//プレイヤーのシェアードポインタ
	std::shared_ptr<Broom> broom_;				//これも消すかもしれない
	std::shared_ptr<DepthOfField> depthOfField_;
	std::shared_ptr<ObjectManager> objManager_;
	std::shared_ptr<ShotManager> shotManager_;
	std::shared_ptr<Tutorial> tutorial_;
	
	std::shared_ptr<CheckCollisionModel> checkCollisionModel_;	//衝突判定を行うクラスのポインタ

	void (GameMain::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ

};

