#pragma once
#include <DxLib.h>
#include <memory>
#include <list>

class Model;
class Shot;

class ShotManager
{
public:
	ShotManager();
	virtual ~ShotManager();

	//更新
	void Update();
	//描画
	void Draw();

	//ショットを放つ
	void Fire(VECTOR framePos,VECTOR playerPos, float height);

private:

	int shotHandle_ = 0;

	std::list<std::shared_ptr<Shot>> shots_;		//モデルポインタのリスト

};

