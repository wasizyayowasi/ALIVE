#include "Camera.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include <algorithm>

namespace {
	constexpr float horizonta_size_of_one_room = 1000.0f;

	//カメラが境界線に到達したときスライド移動するスピード
	constexpr float camera_moveX_speed = 10.0f;
	constexpr float camera_moveZ_speed = 40.0f;
	constexpr float camera_moveY_speed = 40.0f;

	//注視点を逸らすときのどのくらい注視点から逸らすのか
	constexpr float add_focus = 30.0f;

	//カメラの初期ポジション
	const VECTOR init_pos = VGet(0, 300, -600);

	//カメラのZ座標が移動する際、ボーダーライン
	constexpr float tracking_Z_borderline = 1000.0f;

	//カメラのY座標が移動する際、ボーダーラインが有効な範囲
	constexpr float border_range = 100.0f;
}

Camera::Camera():updateFunc_(&Camera::TrackingCameraUpdate)
{

	cameraPos_ = init_pos;

	tempRoom[0] = 1000;
	tempRoom[1] = 1500;
	tempRoom[2] = 2000;

	threshold = tempRoom[0];

	cameraTargetPosZ = -600;
}

Camera::~Camera()
{
}

void Camera::Init()
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
	SetCameraNearFar(5.0f, 5000.0f);
	// カメラの位置、どこを見ているかを設定する
	SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
	// カメラの視野角を設定(ラジアン)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);

}

void Camera::Update(VECTOR playerPos, float playerHeight)
{

	VECTOR distance = {};
	float distanceSize = 2000.0f;

	VECTOR fixedRangePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "FixedCameraRange").pos;

	float fixedRangeSize = VSize(fixedRangePos);

	if (fixedRangeSize > 0.0f) {
		distance = VSub(fixedRangePos, playerPos);

		distanceSize = VSize(distance);

		distanceSize = (std::max)(distanceSize, -distanceSize);
	}

	if (distanceSize < 1500.0f) {
		updateFunc_ = &Camera::FixedPointCamera;
	}
	else {
		updateFunc_ = &Camera::TrackingCameraUpdate;
	}

	(this->*updateFunc_)(playerPos, playerHeight);
}

//プレイヤーを追跡するカメラの更新
void Camera::TrackingCameraUpdate(VECTOR playerPos,float playerHeight)
{
	
	i = (std::max)(i, 0);

	//カメラがプレイヤーを追いかける用にする
	cameraPos_.x = TrackingPosX(playerPos);
	cameraPos_.y = TrackingPosY(playerPos,playerHeight);
	cameraPos_.z = TrackingPozZ(playerPos);

	//プレイヤーがいた位置を見るようにする
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + ((playerPos.y + playerHeight / 2) * 0.1f);
	cameraTarget_.z = (cameraTarget_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);
}

//定点カメラの更新
void Camera::FixedPointCamera(VECTOR playerPos, float playerHeight)
{
	VECTOR fixedRangePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "FixedCameraRange").pos;
	VECTOR fixedPos = ExternalFile::GetInstance().GetCameraGimmickInfo(fixedRangePos, "FixedCamera").pos;

	VECTOR distance = VSub(fixedPos, cameraPos_);

	VECTOR normalise = VNorm(distance);

	VECTOR moveVec = VScale(VScale(normalise, 10.0f), 0.96f);

	float distanceSize = VSize(distance);

	if (distanceSize > 5.0f) {
		cameraPos_ = VAdd(cameraPos_, moveVec);
	}

	//カメラが見る位置をプレイヤーから少しずらす
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + ((playerPos.y + playerHeight / 2) * 0.1f);
	cameraTarget_.z = (cameraTarget_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);
}

//カメラの注視点を逸らす
void Camera::ChangeOfFocus(const InputState& input)
{
	
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input_);

	if (input_.Ry < 0 || input.IsPressed(InputType::upArrow)) {
		cameraTarget_.y += add_focus;
	}
	if (input_.Ry > 0 || input.IsPressed(InputType::downArrow)) {
		cameraTarget_.y -= add_focus;
	}
	if (input_.Rx < 0 || input.IsPressed(InputType::leftArrow)) {
		cameraTarget_.x -= add_focus;
	} 
	if (input_.Rx > 0 || input.IsPressed(InputType::rightArrow)) {
		cameraTarget_.x += add_focus;
	}
}

//プレイヤーを追跡
float Camera::TrackingPosX(VECTOR playerPos)
{

	float boderlinePosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineX").pos.x;
	float gimmickPosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingX").pos.x;
	float distance = 0.0f;

	if (playerPos.x < gimmickPosX && playerPos.x > boderlinePosX - border_range) {
		distance = gimmickPosX - cameraPos_.x;
		moveVecX = distance / camera_moveY_speed;
		moveVecX = moveVecX * 0.96f;

		return cameraPos_.x + moveVecX;
	}

	return (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);

}

float Camera::TrackingPosY(VECTOR playerPos, float playerHeight)
{

	VECTOR boderlinePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineY").pos;
	float gimmickPosY = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingY").pos.y;
	float distance = 0.0f;
	float playerHeadPosY = playerPos.y + playerHeight;

	distance = boderlinePos.x - playerPos.x;

	distance = (std::max)(distance, -distance);

	if (distance < 1000.0f) {
		if (playerHeadPosY < boderlinePos.y + border_range && playerHeadPosY > boderlinePos.y - border_range) {
			distance = gimmickPosY - cameraPos_.y;
			moveVecY = distance / camera_moveY_speed;
			moveVecY = moveVecY * 0.96f;

			return cameraPos_.y + moveVecY;
		}
	}
	

	return (cameraPos_.y * 0.9f) + ((playerHeadPosY + playerHeight) * 0.1f);
}

float Camera::TrackingPozZ(VECTOR playerPos)
{
	
	VECTOR gimmickPos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingZ").pos;
	VECTOR distance = {};

	if (gimmickPos.z <= 0) {
		return init_pos.z;
	}

	distance = VSub(gimmickPos,playerPos);
	float distanceSize = VSize(distance);
	//distance = (std::max)(distance, -distance);

	if (distanceSize < 1500.0f) {
		if (playerPos.z > gimmickPos.z) {
			float targetPosZ = (init_pos.z * 0.3f) + (playerPos.z * 0.7f);
			distance.x = targetPosZ - cameraPos_.z;

			distance.x = distance.x / camera_moveZ_speed;
			moveVecZ = distance.x * 0.96f;

			return cameraPos_.z + moveVecZ;
		}
	}

	distance.z = init_pos.z - cameraPos_.z;
	moveVecZ = distance.z / camera_moveZ_speed;
	moveVecZ = moveVecZ * 0.96f;
	return cameraPos_.z + moveVecZ;

}

void Camera::tempDraW(VECTOR playerPos)
{
	DrawLine3D(VGet(0, 500.0f, 0.0f), VGet(3000.0f, 500.0f, 0.0f), 0xff0000);
	DrawFormatString(0, 16, 0x448844, "%.2f,%.2f,%.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);

	VECTOR gimmickPos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingZ").pos;

//	DrawLine3D(gimmickPos, VGet(gimmickPos.x + 1000.0f, gimmickPos.y, gimmickPos.z), 0xff0000);
//	DrawLine3D(gimmickPos, VGet(gimmickPos.x - 1000.0f, gimmickPos.y, gimmickPos.z), 0xff0000);
}


