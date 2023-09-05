#pragma once
#include "DxLib.h"
#include <memory>

class Camera;

class Broom
{
public:

	Broom();
	virtual ~Broom();

	void writingScreenUpdate(VECTOR playerPos);
	void graphFilterUpdate();

	void update();
	void draw();


private:

	int normalScreen_;			//普通の描画結果を書き込むためのスクリーン
	int highBrightScreen_;		//普通の描画結果から高輝度部分を抜き出して書き込むスクリーン
	int reductioScaleScreen_;	//高輝度部分を縮小した結果を書き込むスクリーン
	int gaussScreen_;			//縮小画像をガウスフィルタでぼかした結果を書き込むスクリーン
	int gaussRation_;			//ガウスフィルタのぼかし度合い
	int reductionScale_;		//縮小するサイズ
	int reductionScreenWidth_;	//ガウスフィルタを掛ける画像の横幅
	int reductionScreenHeight_;	//ガウスフィルタを×画像の縦幅
	
	std::shared_ptr<Camera> camera_;

};

