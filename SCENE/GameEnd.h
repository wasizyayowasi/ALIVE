#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>

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

	int divisionNum_ = 0;												//分割数

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	std::shared_ptr<Camera> camera_;									//カメラのポインタ
	std::shared_ptr<Model> playerModel_;								//プレイヤーモデルのポインタ
	std::shared_ptr<ObjectManager> objManager_;							//オブジェクトマネージャーのポインタ

	std::vector<std::pair<bool,std::shared_ptr<Model>>> corpseModel_;	//死体モデルのポインタ

	void (GameEnd::* updateFunc_)();

};

