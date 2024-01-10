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

	//読み込み
	void LoadFile(bool isLood);

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
	/// 特定のギミックの配置情報を取得する
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns></returns>
	LoadObjectInfo GetSpecifiedGimmickInfo(VECTOR objPos, const char* const name);

	/// <summary>
	/// オブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをすべてまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadObjectInfo() { return loadObjInfo_; }

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
	std::list<LoadObjectInfo> GetSpecifiedInfo(const char* const name);

	LoadObjectInfo GetEnemyInfo(VECTOR playerPos);

	LoadObjectInfo GetDeleteObjInfo(VECTOR pos, const char* const name);

	LoadObjectInfo GetTutorialObjInfo(VECTOR pos);

	/// <summary>
	/// セーブデータの書き出し
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="num">死亡回数</param>
	void SaveDataRewriteInfo(VECTOR pos, int num);

	//セーブデータを初期化する
	void ClearSaveData();

	//ロードしたデータを削除する
	void DeleteData();

private:

	//プレイヤーのステータスに関する情報を読み込む
	void LoadPlayerInfo(const char* const filename);
	//セーブデータを読み込む
	void LoadSaveDataInfo(const char* const filename);
	//オブジェクトの配置情報を読み込む
	void LoadObjectData(const char* const filename, std::unordered_map<std::string, std::list<LoadObjectInfo>>& dataTable);

	//プレイヤーのステータス情報を書き出す
	void RewritePlayerInfo();

	ExternalFile();

	//度数法から弧度法
	VECTOR DegreesToRadians(VECTOR rot);

private:

	PlayerInfo player_ = {};
	SaveData data_ = {};

	ExternalFile(const ExternalFile&) = delete;
	void operator = (const ExternalFile&) = delete;

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadObjInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadGimmickInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadCameraGimmickInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadEnemyInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadTutorialInfo_;
	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadDeleteObjInfo_;

};

