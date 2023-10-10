#include "DepthOfField.h"
#include "../util/game.h"

DepthOfField::DepthOfField()
{

	

	textureWidth = Game::screen_width;
	textureHeight = Game::screen_height;

	normalTexture = MakeScreen(textureWidth, textureHeight, true);
	weakVertigoTexture = MakeScreen(textureWidth, textureHeight, true);
	strongVertigoTexture = MakeScreen(textureWidth, textureHeight, true);

	SetCreateDrawValidGraphChannelNum(1);
	depthTexture = MakeScreen(textureWidth, textureHeight, true);
	SetCreateDrawValidGraphChannelNum(4);

	//	depthNormalMeshVS = LoadVertexShader("");
	//	depthSkinMeshVS = LoadVertexShader("");
	//	depthPS = LoadPixelShader("");
	depthOfFiledPS = LoadPixelShader("DATA/shader/DepthOfField.pso");

}

DepthOfField::~DepthOfField()
{
	//	DeleteShader(depthNormalMeshVS);
	//	DeleteShader(depthSkinMeshVS);
	//	DeleteShader(depthPS);
	//DeleteShader(depthOfFiledPS);
}

void DepthOfField::drawScreen(int ScreenGraphHandle, bool UseDepthShader)
{

}

void DepthOfField::depthOfFieldInit()
{
	//補間範囲とフォーカスの総距離
	depthOfFieldTotalSize = depthOfFieldInterpSize * 2.0f + depthOfFieldFocusSize;

	//津城描画用テクスチャにモデルを通常描画
	drawScreen(normalTexture, false);

	//通常描画の結果に暈しを加えた画像を保存
	GraphFilterRectBlt(normalTexture, weakVertigoTexture, 0, 0, Game::screen_width, Game::screen_height, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200);

	//暈しを加えた場増にさらに暈しを加えて帆zん
	GraphFilterRectBlt(weakVertigoTexture, strongVertigoTexture, 0, 0, Game::screen_width, Game::screen_height, 0, 0, DX_GRAPH_FILTER_GAUSS, 16, 200);

	//深度値描画用テクスチャに深度値を描画
	drawScreen(depthTexture,false);

	//描画先を裏画面に変更
	SetDrawScreen(DX_SCREEN_BACK);

}

void DepthOfField::draw()
{
	SetUseTextureToShader(0, normalTexture);
	SetUseTextureToShader(1, weakVertigoTexture);
	SetUseTextureToShader(2, strongVertigoTexture);
	SetUseTextureToShader(3, depthTexture);

	SetUsePixelShader(depthOfFiledPS);

	//被写界深度のぼかしなしの開始位置を計算
	unblurredRange.x = depthOfFieldInterpSize / depthOfFieldTotalSize;

	//被写界深度のぼかしなしの終了位置を計算
	unblurredRange.y = (depthOfFieldInterpSize + depthOfFieldFocusSize) / depthOfFieldTotalSize;

	SetPSConstF(0, unblurredRange);

	//画面全体に描画する頂点の準備
	vertex_[0].pos = VGet(0.0f,0.0f,0.0f);
	vertex_[1].pos = VGet(Game::screen_width,0.0f,0.0f);
	vertex_[2].pos = VGet(0.0f,Game::screen_height,0.0f);
	vertex_[3].pos = VGet(Game::screen_width,Game::screen_height,0.0f);

	vertex_[0].rhw = 1.0f;
	vertex_[1].rhw = 1.0f;
	vertex_[2].rhw = 1.0f;
	vertex_[3].rhw = 1.0f;

	rightU_ = Game::screen_width / textureWidth;
	bottomV_ = Game::screen_height / textureWidth;

	vertex_[0].u = 0.0f;
	vertex_[1].u = rightU_;
	vertex_[2].u = 0.0f;
	vertex_[3].u = rightU_;

	vertex_[0].v = 0.0f;
	vertex_[1].v = 0.0f;
	vertex_[2].v = bottomV_;
	vertex_[3].v = bottomV_;

	vertex_[4] = vertex_[2];
	vertex_[5] = vertex_[1];

	DrawPolygon2DToShader(vertex_, 2);

}