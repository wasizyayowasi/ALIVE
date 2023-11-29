#pragma once
#include <DxLib.h>
#include <memory>

class Model;

class Shot
{
public:

	Shot(int handle, VECTOR initPos, VECTOR moveVec);
	virtual ~Shot();

	//更新
	void Update();
	//描画
	void Draw();

	//存在しているかどうか取得する
	bool IsEnabled() { return isEnable_; }

private:

	bool isEnable_ = false;						//存在しているか

	VECTOR initialisePos_ = {};					//初期化時のポジション
	VECTOR pos_ = {};							//ポジション
	VECTOR moveVec_ = {};						//移動ベクトル

	std::shared_ptr<Model> model_;				//モデルポインタのリスト

};

