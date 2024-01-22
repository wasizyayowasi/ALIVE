#pragma once
#include "ObjectData.h"
#include <DxLib.h>
#include <memory>
#include <list>
#include <unordered_map>

class ObjectBase;
class Model;
class Player;
class ShotManager;

class ObjectManager
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ObjectManager();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~ObjectManager();

	/// <summary>
	/// メインステージのオブジェクト生成
	/// </summary>
	void MainStageObjectGenerator();

	/// <summary>
	/// オープニングシーンのオブジェクトを生成する
	/// </summary>
	void OpeningStageObjectGenerator();

	/// <summary>
	/// 死体生成
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">オブジェクトの配置データ等</param>
	/// <param name="animNo">初期化時に使用するアニメーション番号</param>
	void DeadPersonGenerator(int handle, LoadObjectInfo objInfo,int animNo);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="player">プレイヤーの参照</param>
	/// <param name="shotManager">ショットマネージャーのスマートポインタ</param>
	void Update(Player& player, std::shared_ptr<ShotManager> shotManager);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pos">プレイヤーポジション</param>
	void Draw(VECTOR PlayerPos);

	/// <summary>
	/// 衝突判定に使用するモデルを取得する
	/// </summary>
	/// <returns>衝突判定を行うモデルスマートポインタのリスト</returns>
	std::list<std::shared_ptr<Model>> GetAllCheckCollModel();

	/// <summary>
	/// baseType別にオブジェクトの生成を振り分ける
	/// </summary>
	/// <param name="baseType">オブジェクトのベースタイプ</param>
	/// <param name="objType">オブジェクトのタイプ</param>
	/// <param name="objInfo">オブジェクトの配置データ</param>
	void SortingObject(ObjectBaseType baseType, ObjectType objType, LoadObjectInfo objInfo);

	/// <summary>
	/// 特定のモデルポインタを取得する
	/// </summary>
	/// <param name="type">取得したいオブジェクトタイプ</param>
	/// <returns>特定のモデルポインタリスト</returns>
	std::list<std::shared_ptr<Model>> GetSpecificModel(ObjectType type);

	/// <summary>
	/// 特定のオブジェクトベースポインタを取得する
	/// </summary>
	/// <param name="type">取得したオブジェクトタイプ</param>
	/// <returns>特定のオブジェクトベースポインタ</returns>
	std::list<std::shared_ptr<ObjectBase>> GetSpecificObject(ObjectType type);

	//衝突判定を行うモデルを追加する
	void AddCheckCollModel();

	/// <summary>
	/// ランダムにポジションを生成する
	/// </summary>
	/// <param name="info">オブジェクト情報</param>
	void RandomPositionGenerator(LoadObjectInfo& info,VECTOR loadObjPos);

	/// <summary>
	/// 円周上のポジションを取得する
	/// </summary>
	/// <param name="info"></param>
	/// <param name="loadObjPos"></param>
	void CircumferencePosition(float angle,VECTOR& infoPos, VECTOR playerPos);

private:
	void EnemyGenerator(int deathCount,VECTOR playerPos);
	//置物生成機
	void OrnamentGenerator(ObjectType objType, LoadObjectInfo objInfo);
	//ギミック生成機
	void GimmickObjectGenerator(ObjectType objType, LoadObjectInfo objInfo);
private:

	std::list<std::shared_ptr<Model>> checkCollList_;

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//オブジェクトをobjectTypeをキーにlistでもつ
};

