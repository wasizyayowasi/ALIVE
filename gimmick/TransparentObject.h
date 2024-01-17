#pragma once
#include "GimmickBase.h"
#include <memory>

class Switch;

class TransparentObject : public GimmickBase
{
public:
	//コンストラクタ
	TransparentObject(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~TransparentObject();

	//更新
	void Update(Player& player)override;
	//描画
	void Draw();

	//天秤モデルと死体との衝突判定を行う
	void HitColl(std::shared_ptr<ObjectBase> deadPerson);

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

private:

	int materialNum_ = 0;								//このクラスでインスタンス化される3Dモデルのマテリアルの数を保管する

	float alphaValue_ = 0.0f;							//透過値を保管する

	std::shared_ptr<Switch> switch_;

};

