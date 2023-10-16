#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	//ファイルパスでモデルクラスをインスタンス化するコンストラクタ
	CharacterBase(const char* fileName, LoadObjectInfo objInfo);
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	CharacterBase(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~CharacterBase() {};

	//更新
	virtual void Update(Player& player) override;
	//描画
	virtual void Draw();

protected:

	int animNo_ = 0;				//アニメーション番号

	bool isAnimLoop_ = false;		//アニメーションをループ再生させるか

};

