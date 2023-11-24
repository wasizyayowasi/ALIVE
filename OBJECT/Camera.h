#pragma once
#include <DxLib.h>

class InputState;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	//初期化用
	void Init();

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
	/// <param name="input">入力情報</param>
	void ChangeOfFocus(const InputState& input);

	/// <summary>
	/// プレイヤーの追跡
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	float TrackingPosX(VECTOR playerPos);

	float TrackingPosY(VECTOR playerPos, float playerHeight);

	float TrackingPozZ(VECTOR playerPos);

	VECTOR GetPos() { return cameraPos_; }
	VECTOR GetTarget() { return cameraTarget_; }

	void tempDraW(VECTOR playerPos);

private:

	int i = 0;
	float cameraTargetPosZ = 0.0f;
	float moveVecZ = 0.0f;
	float moveVecY = 0.0f;
	float moveVecX = 0.0f;

	DINPUT_JOYSTATE input_ = {};

	float threshold;
	float tempRoom[3];
	float fixedPointCameraDestinationPosX = 0.0f;		//定点カメラの移動先で座標Xを保管する変数

	VECTOR cameraPos_;									//カメラのポジション
	VECTOR cameraTarget_ = {0, 0, 0};					//カメラのターゲット

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

