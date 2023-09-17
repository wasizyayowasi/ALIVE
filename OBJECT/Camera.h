#pragma once
#include "DxLib.h"

class Camera
{
public:

	Camera();
	virtual ~Camera();

	void trackingCameraUpdate(VECTOR playerPos);
	void fixedPointCamera(VECTOR playerPos);

private:

	float fixedPointCameraDestinationPosX = 0.0f;		//定点カメラの移動先で座標Xを保管する変数

	VECTOR cameraPos_ = {0, 300, -800};					//カメラのポジション

};

