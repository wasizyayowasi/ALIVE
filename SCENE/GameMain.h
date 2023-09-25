#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include <memory>
#include <vector>

class Camera;
class Player;
class Broom;
class DepthOfField;
class Model;
class CharacterBase;
class GimmickBase;

class GameMain : public SceneBase
{
public:
	///コンストラクタ
	GameMain(SceneManager& manager);
	//デストラクタ
	virtual ~GameMain();

	//更新
	void update(const InputState& input);
	//描画
	void draw();

private:

	//消すかもしれない
	void fadeInUpdate(const InputState& input);
	void normalUpdate(const InputState& input);
	void fadeOutUpdate(const InputState& input);

	//ここも消すかもしれない
	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Camera> camera_;			//カメラのシェアードポインタ
	std::shared_ptr<Player> player_;			//プレイヤーのシェアードポインタ
	std::shared_ptr<Broom> broom_;				//これも消すかもしれない
	std::shared_ptr<DepthOfField> depthOfField_;//これも消すかもしれない
	std::shared_ptr<CharacterBase> enemy_;
	std::shared_ptr<GimmickBase> switch_;
	
	std::vector<std::shared_ptr<Model>> models_;//衝突判定を行う予定のモデルをひとまとめにする配列

	void (GameMain::* updateFunc_)(const InputState& input);		//メンバ関数ポインタ

};

