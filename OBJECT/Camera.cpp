#include "Camera.h"
#include "../util/InputState.h"
#include <algorithm>

namespace {
	constexpr float horizonta_size_of_one_room = 1000.0f;

	constexpr float camera_sride_speed = 10.0f;

	constexpr float add_focus = 50.0f;
}

Camera::Camera()
{
	tempRoom[0] = 1000;
	tempRoom[1] = 1500;
	tempRoom[2] = 2000;

	threshold = tempRoom[0];
}

Camera::~Camera()
{
}

void Camera::init()
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

//プレイヤーを追跡するカメラの更新
void Camera::trackingCameraUpdate(VECTOR playerPos)
{
	
	i = (std::max)(i, 0);

	tracking(playerPos);

	//カメラがプレイヤーを追いかける用にする
	cameraPos_.y = ((300.0f * 0.9f) + (playerPos.y * 0.1f));
	cameraPos_.z = playerPos.z - 800.0f;

	//プレイヤーがいた位置を見るようにする
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = (cameraTarget_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);

}

//定点カメラの更新
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

//カメラの注視点を逸らす
void Camera::changeOfFocus(const InputState& input)
{
	if (input.isPressed(InputType::upArrow)) {
		cameraTarget_.y += add_focus;
	}
	else if (input.isPressed(InputType::downArrow)) {
		cameraTarget_.y -= add_focus;
	}
	else if (input.isPressed(InputType::leftArrow)) {
		cameraTarget_.x -= add_focus;
	} 
	else if (input.isPressed(InputType::rightArrow)) {
		cameraTarget_.x += add_focus;
	}
}

//プレイヤーを追跡
void Camera::tracking(VECTOR playerPos)
{

	bool overRightEndOfTheRoom = playerPos.x > threshold;
	bool overLeftEndOfTheRoom = playerPos.x > threshold - tempRoom[i];

	bool belowRightEndOfTheRoom = playerPos.x < threshold;
	bool belowLeftEndOfTheRoom = playerPos.x < threshold - tempRoom[i];


	//ある一定のラインに来るとカメラの位置を右方向(xがプラスの方向)に移動させる
	if (overRightEndOfTheRoom && overLeftEndOfTheRoom) {
		cameraPos_.x = (std::min)(cameraPos_.x + camera_sride_speed, threshold + 200.0f);
		if (playerPos.x >= threshold + 50.0f) {
			i++;
			threshold += tempRoom[i];
		}
	}
	//ある一定のラインに来るとカメラの位置を左方向(xがマイナスの方向)に移動させる
	else if (belowRightEndOfTheRoom && belowLeftEndOfTheRoom) {
		if (playerPos.x < threshold - tempRoom[i] - 50.0f) {
			threshold -= tempRoom[i];
			i--;
		}
		if (cameraPos_.x > threshold - tempRoom[i] - 200.0f) {
			cameraPos_.x -= camera_sride_speed;
		}
	}
	//通常時のカメラ移動
	else {
		cameraPos_.x = (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);
	}
}


