#pragma once
#include "ObjectData.h"
#include <list>
#include <string>
#include <memory>
#include <DxLib.h>
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
	/// エンディングシーンのオブジェクトを生成する
	/// </summary>
	void EndStageObjectGenerator();

	/// <summary>
	/// 死体生成
	/// </summary>
	/// <param name="handle">モデルハンドル</param>
	/// <param name="objInfo">オブジェクトの配置データ等</param>
	/// <param name="animNo">初期化時に使用するアニメーション番号</param>
	void CorpseGenerator(const int handle, const  LoadObjectInfo objInfo, const int animNo);

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
	void Draw(const VECTOR PlayerPos);

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
	/// <param name="materialType">オブジェクトのマテリアルタイプ</param>
	/// <param name="objInfo">オブジェクトの配置データ</param>
	void SortingObject(const ObjectBaseType baseType, const ObjectType objType, const Material materialType, const LoadObjectInfo objInfo);

	/// <summary>
	/// 特定のモデルポインタを取得する
	/// </summary>
	/// <param name="type">取得したいオブジェクトタイプ</param>
	/// <returns>特定のモデルポインタリスト</returns>
	std::list<std::shared_ptr<Model>> GetSpecificModel(const ObjectType type);

	/// <summary>
	/// 特定のオブジェクトベースポインタを取得する
	/// </summary>
	/// <param name="type">取得したオブジェクトタイプ</param>
	/// <returns>特定のオブジェクトベースポインタ</returns>
	std::list<std::shared_ptr<ObjectBase>> GetSpecificObject(const ObjectType type);

	/// <summary>
	/// 衝突判定を行うモデルを追加する
	/// </summary>
	void AddCheckCollModel();

	/// <summary>
	/// ランダムにポジションを生成する
	/// </summary>
	/// <param name="info">オブジェクト情報</param>
	void RandomPositionGenerator(LoadObjectInfo& info, const VECTOR loadObjPos);

	/// <summary>
	/// 円周上のポジションを取得する
	/// </summary>
	/// <param name="info"></param>
	/// <param name="loadObjPos"></param>
	void CircumferencePosition(const float angle,VECTOR& infoPos, const  VECTOR playerPos);

	/// <summary>
	/// 敵生成
	/// </summary>
	/// <param name="deathCount">今までにプレイヤーが死亡した数</param>
	/// <param name="info">配置データ</param>
	void EnemyGenerator(const int deathCount, const  LoadObjectInfo info);

	/// <summary>
	/// エンディングの敵の生成
	/// </summary>
	/// <param name="deathCount">死んだ回数</param>
	/// <param name="info">配置データ</param>
	void EndEnemyGenerator(const int deathCount, LoadObjectInfo info);
private:
	/// <summary>
	/// 死んだ回数分生成する
	/// </summary>
	/// <param name="deathCount">今までにプレイヤーが死亡した数</param>
	/// <param name="info">配置データ</param>
	void GeneratedForTheNumberOfTimesYouDie(const int deathCount, const  LoadObjectInfo info);

	/// <summary>
	/// 既定の回数分生成する
	/// </summary>
	/// <param name="deathCount">今までにプレイヤーが死亡した数</param>
	/// <param name="str">文字列</param>
	/// <param name="info">配置データ</param>
	void GeneratePredeterminedNumberOfTimes(const int deathCount, const std::string str, const  LoadObjectInfo info);

	/// <summary>
	/// 死んだ回数によって補助足場を生成する
	/// </summary>
	/// <param name="deathCount">死んだ回数</param>
	/// <param name="info">配置データ</param>
	void GenerateCorpseMountain(const int deathCount, const  LoadObjectInfo info);

	/// <summary>
	/// 置物生成機
	/// </summary>
	/// <param name="objType">オブジェクトのタイプ</param>
	/// <param name="materialType">オブジェクトのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	void OrnamentGenerator(const ObjectType objType, const  Material materialType, const  LoadObjectInfo objInfo);

	/// <summary>
	/// ギミック生成機
	/// </summary>
	/// <param name="objType">オブジェクトのタイプ</param>
	/// <param name="materialType">オブジェクトのタイプ</param>
	/// <param name="objInfo">配置データ</param>
	void GimmickObjectGenerator(const ObjectType objType, const  Material materialType, const  LoadObjectInfo objInfo);
private:

	std::list<std::shared_ptr<Model>> checkCollList_;											//衝突判定を行うモデルのリスト
	std::unordered_map<std::string, bool>  usedEnemyList_;										//使用済みエネミーのリスト
	std::unordered_map<std::string, bool>  usedCorpseMtList_;									//使用済みの死体のリスト

	std::unordered_map<ObjectType, std::list<std::shared_ptr<ObjectBase>>> objects_;			//オブジェクトをobjectTypeをキーにlistでもつ
};

