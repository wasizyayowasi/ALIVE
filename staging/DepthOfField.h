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

	int depthNormalMeshVS;			//胴体メッシュ用の深度値描画用頂点シェーダーハンドル
	int depthSkinMeshVS;			//スキニングメッシュ用の深度値描画用頂点シェーダーハンドル
	int depthPS;					//深度値描画用ピクセルシェーダーハンドル
	int depthOfFiledPS;				//被写界深度処理用のピクセルシェーダーハンドル

	int textureWidth;				//テクスチャの横幅
	int textureHeight;				//テクスチャの縦幅
									
	int normalTexture;				//通常描画用のテクスチャ
	int	weakVertigoTexture;			//弱い暈しのテクスチャ
	int	strongVertigoTexture;		//強い暈しのテクスチャ
	int	depthTexture;				//深度値描画用のテクスチャ

	float depthOfFieldTotalSize;
	float depthOfFieldInterpSize = 3000.0f;		//被写界深度の補間サイズ
	float depthOfFieldFocus = 3200.0f;
	float depthOfFieldFocusSize = 500.0f;
	float rightU_;
	float bottomV_;

	FLOAT4 unblurredRange;
	VERTEX2DSHADER vertex_[6];
};

