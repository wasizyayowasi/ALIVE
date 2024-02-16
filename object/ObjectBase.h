#pragma once

#include "ObjectData.h"

#include <DxLib.h>
#include <memory>

class Model;
class Player;

class ObjectBase
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="materialType">マテリアルのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	ObjectBase(int handle,Material materialType, LoadObjectInfo objInfo);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectBase();

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	virtual void Update(Player& player);
	
	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw();

	/// <summary>
	/// 死体に対しての更新
	/// </summary>
	/// <param name="pointer">死体のポインタ</param>
	virtual void UpdateForCorpse(std::shared_ptr<ObjectBase> pointer);

	/// <summary>
	/// 衝突判定を行うモデルを追加する
	/// </summary>
	/// <returns>モデルポインタ</returns>
	virtual std::shared_ptr<Model> AddCollModel();


	//////////////GETTER//////////////

	/// <summary>
	/// モデルのスマートポインタを取得する
	/// </summary>
	/// <returns>モデルポインタ</returns>
	std::shared_ptr<Model> GetModelPointer();

	/// <summary>
	/// ポジションの取得
	/// </summary>
	/// <returns>ポジション</returns>
	VECTOR GetPos() { return pos_; }

	/// <summary>
	/// 現在、このオブジェクトが有効か無効か
	/// </summary>
	/// <returns></returns>
	bool GetIsEnabled() { return isEnable_; };

	/// <summary>
	/// 衝突判定を行うか取得する
	/// </summary>
	/// <returns>衝突判定を行うか</returns>
	bool GetIsCollCheck() { return isCollCheck_; }

	/// <summary>
	/// オブジェクトを運んでいるかどうか取得する
	/// </summary>
	/// <returns>オブジェクトを運んでいるか</returns>
	bool GetIsTransit() { return isTransit_; }


	//////////////SETTER//////////////

	/// <summary>
	/// 持ち運び中フラグの設定
	/// </summary>
	/// <param name="isTransit">true：運んでいる　false：運んでいない</param>
	void SetIsTransit(bool isTransit) { isTransit_ = isTransit; }

	/// <summary>
	/// 現在、このオブジェクトが有効か無効か
	/// </summary>
	/// <param name="isEnable">true：有効　false：無効</param>
	void SetIsEnable(bool isEnable) { isEnable_ = isEnable; }

protected:

	bool isEnable_ = false;						//有効か無効か
	bool isTransit_ = false;					//持ち運び中か
	bool isCollCheck_ = false;					//衝突判定を行うか
	bool isUpdateColl_ = false;					//衝突判定を行う際、衝突判定を行うモデルフレームの更新が必要か

	VECTOR pos_ = { 0.0f,0.0f,0.0f };			//ポジション
	VECTOR rot_ = { 0.0f,0.0f,0.0f };			//回転率
	VECTOR scale_ = { 0.5f,0.5f,0.5f };			//拡縮率

	std::shared_ptr<Model> model_;				//モデルクラスのスマートポインタ
};

