#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <DxLib.h>
#include "../object/PlayerData.h"
#include "../object/ObjectData.h"

struct SaveData {
	VECTOR checkPoint;
	int totalDeathNum;
};

class ExternalFile
{
public:

	~ExternalFile();

	static ExternalFile& GetInstance() {
		static ExternalFile instance;
		return instance;
	}

	/// <summary>
	/// ファイルをまとめて読み込む
	/// </summary>
	void LoadFile();

	/// <summary>
	/// 配置データを読み込む
	/// </summary>
	void LoadArrangementData();

	/// <summary>
	/// セーブデータを読み込む
	/// </summary>
	void LoadSaveData();

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	void LoadFileHandle(std::string name);

	/// <summary>
	/// 特定のギミックの配置情報を取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name);

	/// <summary>
	/// カメラが特殊な動きを行う印(オブジェクト)が
	/// どこにあるかの配置データを取得する
	/// </summary>
	/// <returns></returns>
	LoadObjectInfo GetCameraGimmickInfo(VECTOR playerPos,const char* const name);

	/// <summary>
	/// 指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	LoadObjectInfo GetSpecifiedInfo(const char* const stage,const char* const name);

	/// <summary>
	/// エネミーの配置データを取得する
	/// </summary>
	/// <param name="playerPos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	LoadObjectInfo GetEnemyInfo(VECTOR playerPos);

	/// <summary>
	/// オブジェクトを削除するポイントの配置データを取得する
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	/// <param name="name"></param>
	/// <returns>配置データ</returns>
	LoadObjectInfo GetDeleteObjInfo(VECTOR pos, const char* const name);

	/// <summary>
	/// チュートリアルを表示するポイントの配置データを取得する
	/// </summary>
	/// <param name="pos">プレイヤーのポジション</param>
	/// <returns>配置データ</returns>
	LoadObjectInfo GetTutorialObjInfo(VECTOR pos);

	/// <summary>
	/// セーブデータを削除する
	/// </summary>
	void ClearSaveData();

	/// <summary>
	/// セーブデータの書き出し
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="num">死亡回数</param>
	void SaveDataRewriteInfo(VECTOR pos, int num);

	/// <summary>
	/// プレイヤーに関する情報を取得する
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() { return player_; }

	/// <summary>
	/// savedataを取得する
	/// </summary>
	/// <returns>saveData</returns>
	SaveData GetSaveData() { return data_; }

	/// <summary>
	/// オブジェクトの配置情報を取得
	/// </summary>
	/// <returns>配置データ</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetGimmickInfo() { return loadGimmickInfo_; }

	/// <summary>
	/// メインステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadMainStageObjectInfo() { return loadMainStageObjInfo_; }

	/// <summary>
	/// オープニングステージオブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadOpeningStageObjectInfo() { return loadOpeningStageObjInfo_; }

	/// <summary>
	/// カメラの座標データを取得する
	/// </summary>
	/// <param name="name">名前</param>
	/// <returns>座標データ</returns>
	VECTOR GetCameraTargetPos(std::string name);

	/// <summary>
	/// 指定UIの配置座標を取得する
	/// </summary>
	/// <param name="name">取得したいUIの名前</param>
	/// <returns>座標</returns>
	VECTOR GetUIPos(std::string name);
private:

	/// <summary>
	/// プレイヤーのステータス情報を書き出す 
	/// </summary>
	void RewritePlayerInfo();

	/// <summary>
	/// プレイヤーのステータスに関する情報を読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	void LoadPlayerInfo(const char* const filename);

	/// <summary>
	/// セーブデータを読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	void LoadSaveDataInfo(const char* const filename);

	/// <summary>
	/// オブジェクトの配置情報を読み込む
	/// </summary>
	/// <param name="filename">ファイルの名前</param>
	/// <param name="dataTable">データテーブル</param>
	void LoadObjectData(std::string name, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);


private:

	ExternalFile();

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	PlayerInfo player_ = {};		//プレイヤーのステータス情報
	SaveData data_ = {};			//セーブデータ情報

	std::unordered_map<std::string, int> loadFile_;										//ロードしたファイル
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadMainStageObjInfo_;	//メインステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadOpeningStageObjInfo_;//オープニングステージオブジェクトの配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;		//ギミックの配置データ	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraGimmickInfo_;	//カメラギミックの配置データ		
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEnemyInfo_;			//敵の配置データ
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadTutorialInfo_;		//チュートリアルポイントの配置データ	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadDeleteObjInfo_;		//削除ポイントの配置データ	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraPosInfo_;		//カメラの配置データ	
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadUIInfo_;				//UIの配置データ	
};

