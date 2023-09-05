#pragma once
#include "DxLib.h"

class DepthOfField
{
public:

	DepthOfField();
	virtual ~DepthOfField();

	void drawScreen(int ScreenGraphHandle, bool UseDepthShader);
	void depthOfFieldInit();
	void draw();

private:

	int depthNormalMeshVS;			//���̃��b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
	int depthSkinMeshVS;			//�X�L�j���O���b�V���p�̐[�x�l�`��p���_�V�F�[�_�[�n���h��
	int depthPS;					//�[�x�l�`��p�s�N�Z���V�F�[�_�[�n���h��
	int depthOfFiledPS;				//��ʊE�[�x�����p�̃s�N�Z���V�F�[�_�[�n���h��

	int textureWidth;				//�e�N�X�`���̉���
	int textureHeight;				//�e�N�X�`���̏c��
									
	int normalTexture;				//�ʏ�`��p�̃e�N�X�`��
	int	weakVertigoTexture;			//�ア�򂵂̃e�N�X�`��
	int	strongVertigoTexture;		//�����򂵂̃e�N�X�`��
	int	depthTexture;				//�[�x�l�`��p�̃e�N�X�`��

	float depthOfFieldTotalSize;
	float depthOfFieldInterpSize = 3000.0f;		//��ʊE�[�x�̕�ԃT�C�Y
	float depthOfFieldFocus = 3200.0f;
	float depthOfFieldFocusSize = 500.0f;
	float rightU_;
	float bottomV_;

	FLOAT4 unblurredRange;
	VERTEX2DSHADER vertex_[6];
};

