#pragma once
#include <DxLib.h>

class InputState;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	//�������p
	void Init();

	void Update(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �v���C���[��ǐՂ���J�����̍X�V
	/// </summary>
	/// <param name="input">���͏��</param>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void TrackingCameraUpdate(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// ��_�J����
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	void FixedPointCamera(VECTOR playerPos, float playerHeight);

	/// <summary>
	/// �J�����̒����_����炷
	/// </summary>
	/// <param name="input">���͏��</param>
	void ChangeOfFocus(const InputState& input);

	/// <summary>
	/// �v���C���[�̒ǐ�
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
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
	float fixedPointCameraDestinationPosX = 0.0f;		//��_�J�����̈ړ���ō��WX��ۊǂ���ϐ�

	VECTOR cameraPos_;									//�J�����̃|�W�V����
	VECTOR cameraTarget_ = {0, 0, 0};					//�J�����̃^�[�Q�b�g

	void(Camera::* updateFunc_)(VECTOR pos, float height);

};

