#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
#include <DxLib.h>

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

	/// <summary>
	/// 死体の更新
	/// </summary>
	/// <param name="playerPos">プレイヤーの座標</param>
	void CorpseUpdate(VECTOR playerPos);

	/// <summary>
	/// ホワイトボードの更新
	/// </summary>
	void WhiteBoardUpdate();

	void FadeInUpdate();
	void NormalUpdate();
	void FadeOutUpdate();
private:

	int pigumo21FontHandle_ = -1;										//ピグモ21のフォントハンドル
	int pigumo42FontHandle_ = -1;										//ピグモ42のフォントハンドル
	int divisionNum_ = 0;												//分割数
	int corpseNum_ = 0;													//操作する死体の番号
	int currentDivisionNum_ = 1;										//現在の分割数

	static constexpr int fadeInterval_ = 60;
	int fadeTimer_ = fadeInterval_;
	int fadeValue_ = 255;
	int fadeColor_ = 0x000000;

	
	bool isPutting_ = false;											//置いている
	bool isTurn_ = false;												//回転中
	bool isWentToRightSide_ = true;										//右端に行ったか
	bool boardIsMoving_ = true;											//ボードが動いているか
	bool isResultDisplaying_ = false;									//結果を表示中

	VECTOR moveVec_ = { 1.0f,0.0f,0.0f };								//移動ベクトル

	std::shared_ptr<Camera> camera_;									//カメラのポインタ
	std::shared_ptr<Model> playerModel_;								//プレイヤーモデルのポインタ
	std::shared_ptr<Model> boardModel_;									//ホワイトボードモデルのポインタ
	std::shared_ptr<ObjectManager> objManager_;							//オブジェクトマネージャーのポインタ

	std::vector<int> barGraphHeight_;									//棒グラフの高さ
	std::vector<std::pair<bool,std::shared_ptr<Model>>> corpseModel_;	//死体モデルのポインタ

	void (GameEnd::* updateFunc_)();

};

