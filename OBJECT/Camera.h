#pragma once
#include <DxLib.h>

class Camera
{
public:
	//コンストラクタ
	Camera(VECTOR pos);
	//デストラクタ
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
	void ChangeOfFocus();

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
	/// カメラが目標とする座標を設定する
	/// </summary>
	/// <param name="targetPos">目標位置</param>
	void SetCameraTargetPos(VECTOR targetPos);

	void tempdraw();

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

	/// <summary>
	/// プレイヤーのZ座標を追跡
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	float TrackingPozZ(VECTOR playerPos);

private:

	int i = 0;
	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float moveVecX = 0.0f;

	float elapsedTime_ = 0.0f;

	DINPUT_JOYSTATE input_ = {};

	VECTOR initPos_ = {};						//初期ポジション
	VECTOR pos_ = {};							//カメラのポジション
	VECTOR cameraViewingPos_ = {};				//カメラが見る位置
	VECTOR cameraTargetPos_ = {};				//カメラの目標位置

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

