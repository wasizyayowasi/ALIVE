#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include "DxLib.h"
#include "PlayerData.h"
#include "ObjectData.h"

struct SaveData {
	VECTOR checkPoint;
	int totalDeathNum;
};

class LoadExternalFile
{
public:

	~LoadExternalFile();

	static LoadExternalFile& GetInstance() {
		static LoadExternalFile instance;
		return instance;
	}

	/// <summary>
	/// プレイヤーに関する情報を取得する
	/// </summary>
	/// <returns>playerData</returns>
	PlayerInfo GetPlayerInfo() { return player; }

	/// <summary>
	/// savedataを取得する
	/// </summary>
	/// <returns>saveData</returns>
	SaveData GetSaveData() { return data; }
	
	/// <summary>
	/// オブジェクトの配置、回転率、拡縮率を取得する
	/// </summary>
	/// <returns>オブジェクトの配置データなどをすべてまとめた変数</returns>
	std::unordered_map<std::string, std::list<LoadObjectInfo>> GetLoadObjectInfo() { return loadObjInfo_; }

	/// <summary>
	/// 指定した名前のオブジェクト配置データを返す
	/// </summary>
	/// <param name="name">取得したいオブジェクトの名前</param>
	/// <returns>オブジェクトのデータ</returns>
	std::list<LoadObjectInfo> GetSpecifiedInfo(const char* const name);

	/// <summary>
	/// セーブデータの書き出し
	/// </summary>
	/// <param name="pos">ポジション</param>
	/// <param name="num">死亡回数</param>
	void SaveDataRewriteInfo(VECTOR pos, int num);

	//セーブデータを使用するか
	void LoadSaveFile(bool isContinue);

private:

	//プレイヤーのステータスに関する情報を読み込む
	void LoadPlayerInfo(const char* const filename);
	//セーブデータを読み込む
	void LoadSaveDataInfo(const char* const filename);
	//オブジェクトの配置情報を読み込む
	void LoadObjectData(const char* const filename);

	//プレイヤーのステータス情報を書き出す
	void RewritePlayerInfo();

	LoadExternalFile();

	//度数法から弧度法
	VECTOR DegreesToRadians(VECTOR rot);

private:

	PlayerInfo player;
	SaveData data;

	std::unordered_map<std::string, std::list<LoadObjectInfo>> loadObjInfo_;

};

