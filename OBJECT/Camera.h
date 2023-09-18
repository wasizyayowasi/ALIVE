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
	void changeOfFocus(const InputState& input);

private:

	float fixedPointCameraDestinationPosX = 0.0f;		//定点カメラの移動先で座標Xを保管する変数

	VECTOR cameraPos_ = {0, 300, -800};					//カメラのポジション
	VECTOR cameraTarget_ = {0, 0, 0};					//カメラのターゲット

};

