#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Player;

class ObjectBase
{
public:
	//ファイルパスでモデルクラスをインスタンス化するコンストラクタ
	ObjectBase(const char* const filename);
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	ObjectBase(int handle);
	//デストラクタ
	virtual ~ObjectBase();

	//更新
	virtual void Update(Player& player);
	//描画
	virtual void Draw();

	//現在有効か無効化を取得する
	bool IsEnabled() { return isEnable_; };

	//モデルのスマートポインタを取得する
	std::shared_ptr<Model> GetModelPointer() { return model_; }

protected:

	bool isEnable_ = false;						//有効か無効か

	VECTOR pos_ = { 0.0f,0.0f,0.0f };			//ポジション
	VECTOR rot_ = { 0.0f,0.0f,0.0f };			//回転率
	VECTOR scale_ = { 0.5f,0.5f,0.5f };			//拡縮率

	std::shared_ptr<Model> model_;				//モデルクラスのスマートポインタ

};

