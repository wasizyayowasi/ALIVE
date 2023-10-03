#pragma once
#include "DxLib.h"
#include "ObjectBase.h"
#include <memory>

class CharacterBase : public ObjectBase
{
public:
	//ファイルパスでモデルクラスをインスタンス化するコンストラクタ
	CharacterBase(const char* fileName);
	//作成済みモデルでモデルクラスをインスタンス化するコンストラクタ
	CharacterBase(int handle);
	//デストラクタ
	virtual ~CharacterBase() {};

	//更新
	virtual void update();
	//描画
	virtual void draw();

protected:

	int animNo_ = 0;				//アニメーション番号

	bool isAnimLoop_ = false;		//アニメーションをループ再生させるか

};

