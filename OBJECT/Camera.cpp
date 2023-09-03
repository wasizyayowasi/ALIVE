#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
	/////////////// 3D関連の設定 /////////////
	// Zバッファを使用する
	SetUseZBuffer3D(true);
	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);
	// ポリゴンの裏面を描画しない
	SetUseBackCulling(true);

	//////////////// カメラの設定 //////////////////
	// カメラからどれだけ離れたところ( Near )から、 どこまで( Far )のものを描画するかを設定
	SetCameraNearFar(5.0f, 2800.0f);
	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
}

Camera::~Camera()
{
}

void Camera::trackingCameraUpdate(VECTOR playerPos)
{
	cameraPos_ = playerPos;
	cameraPos_.y += 300.0f;
	cameraPos_.z -= 600.0f;

	SetCameraPositionAndTarget_UpVecY(cameraPos_, playerPos);
}

void Camera::fixedPointCamera(VECTOR playerPos)
{
	//一定範囲を出たらカメラが次の場所へヌルっと動く
	if (playerPos.x >= 500) {
		fixedPointCameraDestinationPosX = 1000;
		cameraPos_.x = (std::min)(cameraPos_.x + 20.0f, fixedPointCameraDestinationPosX);
	}
	else {
		fixedPointCameraDestinationPosX = 0;
		cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
	}

	SetCameraPositionAndTarget_UpVecY(cameraPos_, playerPos);
}

