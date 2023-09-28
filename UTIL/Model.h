#pragma once
#include "DxLib.h"

class Model
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fileName">���f���������Ă���t�@�C���p�X</param>
	Model(const char* fileName);
	//duplicate�p�R���X�g���N�^
	Model(int orgModel);

	//�f�X�g���N�^
	virtual ~Model();

	/// <summary>
	/// collision�t���[����collision�Ƃ��Ďg��
	/// </summary>
	/// <param name="isUse">�g�p���邩</param>
	/// <param name="isNeedUpdate">�X�V���K�v��</param>
	void setUseCollision(bool isUse, bool isNeedUpdate);

	//�X�V
	void update();
	//�`��
	void draw();

	/// <summary>
	/// ���f�����擾����
	/// </summary>
	/// <returns>���f���n���h����Ԃ�</returns>
	int getModelHandle() const { return modelHandle_; }
	/// <summary>
	/// collision�t���[�����擾����
	/// </summary>
	/// <returns>collision�t���[����Ԃ�</returns>
	int getColFrameIndex() const { return colFrameIndex_; }
	/// <summary>
	/// �|�W�V�������擾����
	/// </summary>
	/// <returns>�|�W�V������Ԃ�</returns>
	VECTOR getPos() const { return pos_; }
	/// <summary>
	/// �A�j���[�V�����̍Đ����Ԃ��擾����
	/// </summary>
	/// <returns>��A�j���[�V�����̍Đ�����</returns>
	float getAnimTotalTime()const { return animNext_.totalTime; }

	/// <summary>
	/// �|�W�V������ݒ肷��
	/// </summary>
	/// <param name="pos">�|�W�V����</param>
	void setPos(VECTOR pos);
	/// <summary>
	/// �p�x��ݒ肷��
	/// </summary>
	/// <param name="rot">�p�x</param>
	void setRot(VECTOR rot);
	/// <summary>
	/// �T�C�Y��ݒ肷��
	/// </summary>
	/// <param name="scale">�g�k��</param>
	void setScale(VECTOR scale);
	/// <summary>
	/// collision�Ƃ��Ďg�������t���[�����w�肷��
	/// </summary>
	/// <param name="collFrameName">collision�Ƃ��Ďg�������t���[�����B�Ȃ���΋�</param>
	void setCollFrame(const char* collFrameName = "");

	/// <summary>
	/// �A�j���[�V������ݒ肷��
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">���[�v�Đ����邩</param>
	/// <param name="IsForceChange">�����I�ɕύX���邩</param>
	void setAnimation(int animNo,bool isLoop,bool IsForceChange);

	/// <summary>
	/// �A�j���[�V������ύX����
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="isLoop">���[�v�Đ����邩</param>
	/// <param name="isForceChange">�����I�ɕύX���邩</param>
	/// <param name="changeFrame">�A�j���[�V������ύX����ۂɉ��b�����ĕω�����̂�</param>
	void changeAnimation(int animNo, bool isLoop, bool isForceChange, int changeFrame);

	/// <summary>
	/// �A�j���[�V�����̏I�����擾����
	/// </summary>
	/// <returns>�A�j���[�V�������I�������</returns>
	bool isAnimEnd();

	/// <summary>
	/// �A�j���[�V�����ԍ��̍Ō�̃t���[����ݒ肷��
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	void setAnimEndFrame(int animNo);

	/// <summary>
	/// �w�肵���t���[���̍��W���擾����
	/// </summary>
	/// <param name="animNo">�A�j���[�V�����ԍ�</param>
	/// <param name="frameName">�擾�������A�j���[�V������</param>
	/// <returns>���W</returns>
	VECTOR getAnimFrameLocalPosition(int animNo,const char* frameName);

private:
	struct AnimData {
		int animNo;
		int attachNo;
		float totalTime;
		bool isLoop;
	};
private:
	/// <summary>
	/// �A�j���[�V�����f�[�^������������
	/// </summary>
	/// <param name="anim"></param>
	void clearAnimData(AnimData& anim);

	/// <summary>
	/// �A�j���[�V�����̍X�V
	/// </summary>
	/// <param name="anim"></param>
	/// <param name="dt"></param>
	void updateAnim(AnimData anim, float dt = 1.0f);

	//�A�j���[�V�����̃u�����h
	void updateAnimBlendRate();
private:
	int modelHandle_ = -1;

	bool isUseCollision_ = false;

	bool isUpdateCollsion_ = false;

	int colFrameIndex_ = 0;

	AnimData animPrev_;
	AnimData animNext_;

	int animChangeFrame_ = 0;
	int animChangeFrameTotal_ = 0;

	VECTOR pos_;

};

