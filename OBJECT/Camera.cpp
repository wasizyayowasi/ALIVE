#include "Camera.h"
#include "../util/InputState.h"
#include <algorithm>

namespace {
	constexpr float horizonta_size_of_one_room = 1000.0f;
}

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

	tempRoom[0] = 1000;
	tempRoom[1] = 1500;
	tempRoom[2] = 2000;

	temp = tempRoom[0];

}

Camera::~Camera()
{
}

void Camera::trackingCameraUpdate(const InputState& input,VECTOR playerPos)
{
	
	cameraPos_.x = (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraPos_.y = ((300.0f * 0.9f) + (playerPos.y * 0.1f));
	cameraPos_.z = -800.0f;

	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = 0;

	//changeOfFocus(input);
	//仮：1800で急激に移動するようにする

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);

	DrawFormatString(0, 80, 0x448844, "x : %.2f,y : %.2f,z : %.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);

}

void Camera::fixedPointCamera(VECTOR playerPos)
{
	//一定範囲を出たらカメラが次の場所へヌルっと動く

	//プレイヤーがいる部屋の番号を取得
	int playerRoomNum = (playerPos.x + horizonta_size_of_one_room / 2 ) / (horizonta_size_of_one_room) ;
	//カメラがいる部屋の番号を取得する
	int cameraRoomNum = fixedPointCameraDestinationPosX / (horizonta_size_of_one_room);
	//プレイヤーとカメラがいる部屋の番号が違ったらカメラのX座標変数(仮)をプレイヤーがいる部屋の番号に変える
	if (playerRoomNum != cameraRoomNum) {
		fixedPointCameraDestinationPosX = horizonta_size_of_one_room * playerRoomNum;
	}

	//カメラの目標位置からカメラのポジションを引く
	float sub = fixedPointCameraDestinationPosX - cameraPos_.x;

	//カメラのポジションと目標位置が同じではなかったら
	if (cameraPos_.x != fixedPointCameraDestinationPosX) {
		//カメラのポジションを移動させる
		if (sub > 0.0f) {
			cameraPos_.x = (std::min)(cameraPos_.x + 20.0f, fixedPointCameraDestinationPosX);
		}
		else {
			cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
		}
	}

	//カメラが見る位置をプレイヤーから少しずらす
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = 0;

	DrawFormatString(0, 80, 0x448844, "x : %.2f,y : %.2f,z : %.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);
}

void Camera::changeOfFocus(const InputState& input)
{
	if (input.isPressed(InputType::up)) {
		cameraTarget_.y += 100.0f;
	}
	else if (input.isPressed(InputType::left)) {
		cameraTarget_.x -= 100.0f;
	} 
	else if (input.isPressed(InputType::right)) {
		cameraTarget_.x += 100.0f;
	}
	else if (input.isPressed(InputType::down)) {
		cameraTarget_.y -= 100.0f;
	}
}

void Camera::tempcamera(VECTOR playerPos)
{

	int roomsize = temp;

	if (playerPos.x > temp && playerPos.x > temp - tempRoom[i]) {
		cameraPos_.x = (std::min)(cameraPos_.x + 5.0f, temp + 200.0f);
		if (playerPos.x >= temp + 200.0f) {
			i++;
			temp += tempRoom[i];
		}
	}
	else if (playerPos.x < temp - tempRoom[i] && playerPos.x < temp) {
		cameraPos_.x = (std::max)(cameraPos_.x - 5.0f, temp - 200.0f);
		if (playerPos.x >= temp - tempRoom[i] - 200.0f) {
			temp -= tempRoom[i];
			i--;
		}
	}
	else {
		cameraPos_.x = (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);
	}
	
	cameraPos_.y = ((300.0f * 0.9f) + (playerPos.y * 0.1f));
	cameraPos_.z = -800.0f;

	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = 0;

	

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);

	DrawFormatString(0, 80, 0x448844, "x : %.2f,y : %.2f,z : %.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);
}


