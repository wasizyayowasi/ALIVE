#pragma once
#include "SceneBase.h"
#include <memory>

class Model;
class Camera;
class ObjectManager;

class GameEnd : public SceneBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">シーンマネージャーの参照</param>
	GameEnd(SceneManager& manager);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameEnd();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();

	/// <summary>
	/// 終了
	/// </summary>
	void End();

	//更新
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	void fadeInUpdate();
	void normalUpdate();
	void fadeOutUpdate();
private:

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Camera> camera_;			//カメラのポインタ
	std::shared_ptr<Model> playerModel_;		//モデルのポインタ
	std::shared_ptr<ObjectManager> objManager_;	//オブジェクトマネージャーのポインタ

	void (GameEnd::* updateFunc_)();

};

