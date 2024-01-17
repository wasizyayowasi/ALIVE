#pragma once
#include <DxLib.h>
#include "ObjectData.h"
#include <memory>
#include <string>

class Model;
class Player;

class ObjectBase
{
public:
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	ObjectBase(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~ObjectBase();

	//更新
	virtual void Update(Player& player);
	//描画
	virtual void Draw();

	//衝突判定
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

	//ポジションの取得
	VECTOR GetPos() { return pos_; }

	//現在有効か無効化を取得する
	bool IsEnabled() { return isEnable_; };

	//衝突判定を行うか
	bool IsCollCheck() { return isCollCheck_; }

	//持ち運び中か
	bool IsTransit() { return isTransit_; }

	//持ち運び中フラグの設定
	void SetIsTransit(bool isTransit) { isTransit_ = isTransit; }

	//有効なオブジェクト化無効なオブジェクト化を設定する
	void SetIsEnable(bool isEnable) { isEnable_ = isEnable; }

	//モデルのスマートポインタを取得する
	std::shared_ptr<Model> GetModelPointer() { return model_; }

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

	//名前を取得する
	std::string GetName() { return name_; }

protected:

	bool isEnable_ = false;						//有効か無効か
	bool isCollCheck_ = false;					//衝突判定を行うか
	bool isTransit_ = false;					//持ち運び中か

	VECTOR pos_ = { 0.0f,0.0f,0.0f };			//ポジション
	VECTOR rot_ = { 0.0f,0.0f,0.0f };			//回転率
	VECTOR scale_ = { 0.5f,0.5f,0.5f };			//拡縮率

	std::string name_;							//名前

	std::shared_ptr<Model> model_;				//モデルクラスのスマートポインタ

};

