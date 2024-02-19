#pragma once
#include <DxLib.h>

class Camera
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="pos">初期座標</param>
	/// <param name="viewPos">見る場所</param>
	Camera(VECTOR pos,VECTOR viewPos);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Camera();

	/// <summary>
	/// カメラの初期化
	/// </summary>
	/// <param name="targetPos">カメラが見る位置</param>
	void Init(VECTOR targetPos);

	/// <summary>
	/// カメラの更新
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="playerHeight">プレイヤーの身長</param>
	void Update(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// プレイヤーを追跡するカメラの更新
	/// </summary>
	/// <param name="input">入力情報</param>
	/// <param name="playerPos">プレイヤーのポジション</param>
	void TrackingCameraUpdate(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// 定点カメラ
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	void FixedPointCamera(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// カメラの注視点を逸らす
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <param name="playerHeight">プレイヤーの身長</param>
	void ChangeOfFocus(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// 目標のポジションまで移動する
	/// </summary>
	void EasingMoveCamera();

	/// <summary>
	/// カメラのポジションを取得する
	/// </summary>
	/// <returns>カメラの座標</returns>
	VECTOR GetPos() { return pos_; }

	/// <summary>
	/// カメラが見る位置を取得する
	/// </summary>
	/// <returns>カメラが見る位置</returns>
	VECTOR GetTarget() { return cameraViewingPos_; }

	/// <summary>
	/// カメラが移動中か取得する
	/// </summary>
	/// <returns>true : 移動中　 false : 移動してない</returns>
	bool GetMoving() { return isMoving_; }

	/// <summary>
	/// カメラが目標とする座標と見る位置を設定する
	/// </summary>
	/// <param name="targetPos">目標位置</param>
	/// <param name="targetViewPos">見る位置</param>
	/// <param name="upVec">カメラの上方向ベクトル</param>
	void SetCameraTargetPosAndView(VECTOR targetPos, VECTOR targetViewPos, VECTOR upVec);

	/// <summary>
	/// 経過時間を0に戻す
	/// </summary>
	void LesetElapsedTime() { elapsedTime_ = 0.0f; }

	void DebugDraw();

private:

	/// <summary>
	/// プレイヤーのX座標を追跡
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	float TrackingPosX(VECTOR playerPos);

	/// <summary>
	/// プレイヤーのY座標を追跡
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	float TrackingPosY(VECTOR playerPos, float playerHeight);

private:

	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float moveVecX = 0.0f;

	float elapsedTime_ = 0.0f;

	bool isMoving_ = false;

	DINPUT_JOYSTATE input_ = {};

	VECTOR pos_ = {};							//カメラのポジション
	VECTOR upVec_ = {0,1,0};					//カメラの見る目標位置
	VECTOR cameraViewingPos_ = {};				//カメラが見る位置

	VECTOR cameraTargetPos_ = {};				//カメラの目標位置
	VECTOR targetUpVec_ = {};					//カメラの目標上方向
	VECTOR cameraTargetViewPos_ = {};			//カメラの見る目標位置

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

