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
	/// オープニングのカメラ
	/// </summary>
	void OpeningCameraUpdate();

	VECTOR GetPos() { return pos_; }
	VECTOR GetTarget() { return cameraTarget_; }

	

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

	DINPUT_JOYSTATE input_ = {};

	VECTOR pos_ = {};							//カメラのポジション
	VECTOR cameraTarget_ = {0, 0, 0};			//カメラのターゲット
	VECTOR initPos_ = {};						//初期ポジション

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

