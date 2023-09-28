#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>
#include <unordered_map>

class Camera;
class Player;
class Broom;
class DepthOfField;
class Model;
class CharacterBase;
class GimmickBase;
class Steelyard;


class GameMain : public SceneBase
{
public:
	///コンストラクタ
	GameMain(SceneManager& manager,bool continuation);
	//デストラクタ
	virtual ~GameMain();

	//初期化用関数
	void init();
	//削除用関数
	void end();

	//更新
	void update(const InputState& input);
	//描画
	void draw();

private:

	int makeScreenHandle_ = -1;				//makeScreen用ハンドル
	int totalDeathNum_ = 0;					//ゲーム開始からの総死亡数

	bool isContinuation_ = false;			//続きからか初めからか

	//消すかもしれない
	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	VECTOR checkPoint_ = { 0.0f,0.0f, 0.0f };

	std::shared_ptr<Camera> camera_;			//カメラのシェアードポインタ
	std::shared_ptr<Player> player_;			//プレイヤーのシェアードポインタ
	std::shared_ptr<Broom> broom_;				//これも消すかもしれない
	std::shared_ptr<DepthOfField> depthOfField_;//これも消すかもしれない
	std::shared_ptr<CharacterBase> enemy_;
	std::vector<std::shared_ptr<GimmickBase>> gimmick_;
	
	std::vector<std::shared_ptr<Model>> models_;//衝突判定を行う予定のモデルをひとまとめにする配列

	void (GameMain::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ

};

