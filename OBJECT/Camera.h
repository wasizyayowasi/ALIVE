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

	VECTOR getPos() { return cameraPos_; }
	int gettemp() { return temp; }
	VECTOR getTarget() { return cameraTarget_; }

	void tempcamera(VECTOR playerPos);

private:

	int i = 0;

	float temp;
	float tempRoom[3];
	float fixedPointCameraDestinationPosX = 0.0f;		//定点カメラの移動先で座標Xを保管する変数

	VECTOR cameraPos_ = {0, 270, -800};					//カメラのポジション
	VECTOR cameraTarget_ = {0, 0, 0};					//カメラのターゲット

};

