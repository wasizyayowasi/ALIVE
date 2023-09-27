#pragma once
#include "DxLib.h"

class InputState;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	void trackingCameraUpdate(const InputState& input,VECTOR playerPos);
	void fixedPointCamera(VECTOR playerPos);

	void changeOfFocus(const InputState& input);							//カメラのターゲットの位置を逸らす
	void tracking(VECTOR playerPos);

	VECTOR getPos() { return cameraPos_; }
	VECTOR getTarget() { return cameraTarget_; }

private:

	int i = 0;

	float threshold;
	float tempRoom[3];
	float fixedPointCameraDestinationPosX = 0.0f;		//定点カメラの移動先で座標Xを保管する変数

	VECTOR cameraPos_ = {0, 270, -800};					//カメラのポジション
	VECTOR cameraTarget_ = {0, 0, 0};					//カメラのターゲット

};

